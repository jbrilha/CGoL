#include "automaton.hpp"

Automaton::Automaton(std::string path_str, GLFWwindow *window, int square_size,
                     glm::vec3 color, bool solid_colors)
    : square_size(square_size), cell_size(square_size + GAP), window(window),
      plague(false), solid_colors(solid_colors), main_color(color),
      shader_program((path_str + "/../shaders/vert.glsl").c_str(),
                     (path_str + "/../shaders/frag.glsl").c_str()) {

    glfwGetWindowSize(window, &win_width, &win_height);

    hei_margin = (win_height % cell_size);
    wid_margin = (win_width % cell_size);

    cell_count = ((win_width / cell_size) * (win_height / cell_size));

    rows = (win_height / cell_size);
    cols = (win_width / cell_size);

    cells = std::vector<int>(cell_count, 0);
    update_cells = cells;

    origin_x = -1.f + pxls::to_float(cell_size + wid_margin, win_width);
    origin_y = 1.f - pxls::to_float(cell_size + hei_margin, win_height);

    colors.push_back(black);
    colors.push_back(main_color);

    set_shaders();
}

Automaton::~Automaton() {
    cells.clear();
    update_cells.clear();
    glDeleteProgram(shader_program.program_ID);
}

void Automaton::update_grid() {
    rows = win_height / cell_size;
    cols = win_width / cell_size;

    hei_margin = win_height % cell_size;
    wid_margin = win_width % cell_size;

    int new_cell_count = (win_width / cell_size) * (win_height / cell_size);
    std::vector<int> new_cells(new_cell_count, 0);
    if (new_cell_count > cell_count) {
        int offset = (new_cell_count - cell_count) / 2;

        std::copy(cells.begin(), cells.end(), new_cells.begin() + offset);

        cells = new_cells;
    } else {
        std::copy(cells.begin(), cells.begin() + new_cell_count,
                  new_cells.begin());
        cells = new_cells;
    }
    cell_count = new_cell_count;
    update_cells = std::vector<int>(cell_count, 0);

    origin_x = -1.f + pxls::to_float(cell_size + wid_margin, win_width);
    origin_y = 1.f - pxls::to_float(cell_size + hei_margin, win_height);

    set_shaders();
}

void Automaton::update_dimensions(int win_width, int win_height) {
    this->win_width = win_width;
    this->win_height = win_height;

    update_grid();
}

void Automaton::update_square_size(int square_size) {
    this->square_size = square_size;
    cell_size = square_size + GAP;

    update_grid();
}

void Automaton::set_shaders() {
    shader_program.use();

    std::vector<glm::vec2> offsets = std::vector<glm::vec2>(cell_count);
    int i = 0;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            offsets[i++] = {
                origin_x + (2 * pxls::to_float(cell_size, win_width)) * col,
                origin_y - (2 * pxls::to_float(cell_size, win_height)) * row};
        }
    }

    glGenBuffers(2, instance_VBOs);
    glBindBuffer(GL_ARRAY_BUFFER, instance_VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * (cell_count),
                 offsets.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, instance_VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * (cell_count), cells.data(),
                 GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    set_square_vertices();

    unsigned int quad_VBO;
    glGenVertexArrays(1, &quad_VAO);
    glGenBuffers(1, &quad_VBO);
    glBindVertexArray(quad_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
    glBufferData(GL_ARRAY_BUFFER, quad_vertices.size() * sizeof(glm::vec2),
                 quad_vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);

    // also set instance data
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,
                 instance_VBOs[0]); // this attribute comes from a different
                                    // vertex buffer
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(
        1, 1); // tell OpenGL this is an instanced vertex attribute.

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER,
                 instance_VBOs[1]); // this attribute comes from a different
                                    // vertex buffer
    // glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(int), (void *)0);
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(int), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(
        2, 1); // tell OpenGL this is an instanced vertex attribute.
}

void Automaton::set_square_vertices() {
    float x_vert = pxls::to_float(square_size, win_width);
    float y_vert = pxls::to_float(square_size, win_height);

    quad_vertices = {
        glm::vec2(x_vert, y_vert),
        glm::vec2(x_vert, -y_vert),
        glm::vec2(-x_vert, -y_vert),
        glm::vec2(-x_vert, y_vert),
    };
}

void Automaton::update_states() {
    glBindBuffer(GL_ARRAY_BUFFER, instance_VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * (cell_count), cells.data(),
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Automaton::set_value(double x_pos, double y_pos, int val, int radius,
                          bool circular) {
    if (x_pos < (double)wid_margin / 2 ||
        x_pos > win_width - (double)wid_margin / 2 ||
        y_pos < (double)hei_margin / 2 ||
        y_pos > win_height - (double)hei_margin / 2) {

        return;
    }

    bool update = false;

    int col = (floor(((int)x_pos - GAP - wid_margin / 2) % (int)(win_width))) /
              cell_size;
    int row = (floor(((int)y_pos - GAP - hei_margin / 2) % (int)(win_height))) /
              cell_size;
    int offset = row * cols + col;

    if (radius == 0) {
        if (cells[offset] == SOLID && val)
            return;

        cells[offset] = val;
        update_cells[offset] = val;
        update = true;
    } else {
        for (int r = row - radius; r < row + radius + 1; r++) {
            for (int c = col - radius; c < col + radius + 1; c++) {
                offset = r * cols + c;

                if (r >= 0 && r < rows && c < cols && c >= 0 &&
                    (val == 0 || cells[offset] != SOLID) &&
                    (!circular || // cursor check for short circuiting.
                                  // micro optimizations babyyyy
                     sqrt(pow(abs(r - row), 2) + pow(abs(c - col), 2)) <=
                         radius + 0.1)) {

                    cells[offset] = val;
                    update_cells[offset] = val;
                    update = true;
                }
            }
        }
    }

    if (update)
        update_states();
}

void Automaton::fill_random() {
    for (int &cell : cells) {
        cell = (rand() % 100) < 20 ? 1 : 0;
    }

    update_states();
}

void Automaton::draw() {
    shader_program.use();

    glBindVertexArray(quad_VAO);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, quad_vertices.size(), cell_count);
    glBindVertexArray(0);
}

void Automaton::clear(bool all) {
    if (all) {
        cells = std::vector<int>(cell_count, 0);
    } else {
        for (int &cell : cells) {
            if (cell && cell != SOLID)
                cell = 0;
        }
    }
    update_cells = cells;

    update_states();
}

void Automaton::set_cell_colors() {
    shader_program.set_bool("solid_colors", solid_colors);

    for (int i = 0; i < (int)colors.size(); i++) {
        shader_program.set_vec3(("colors[" + std::to_string(i) + "]"),
                                colors[i]);
    }
}

void Automaton::save() {
    std::ofstream output("save.txt");

    if (!output.is_open()) {
        std::cout << "FAILED TO CREATE SAVE FILE" << std::endl;
    }

    for (int cell : cells) {
        std::string cell_str = std::to_string(cell);
        output.write(cell_str.c_str(), cell_str.size());
    }

    output.close();
}

void Automaton::load() {
    std::ifstream input("save.txt");

    if (!input.is_open()) {
        std::cout << "FAILED TO READ SAVE FILE" << std::endl;
    }

    std::string cells_str;
    getline(input, cells_str);
    for (int i = 0; i < (int)cells_str.length() && i < cell_count; i++) {
        cells[i] = cells_str[i] - 0x30;
    }
    update_cells = cells;

    input.close();

    update_states();
}

// void Automaton::change_cursor_shape() { circular_cursor = !circular_cursor; }
void Automaton::toggle_plague() { plague = !plague; }
int Automaton::get_cell_count() { return cell_count; }
int Automaton::get_rows() { return rows; }
int Automaton::get_cols() { return cols; }
int Automaton::get_square_size() { return square_size; }
int Automaton::get_cell_size() { return cell_size; }
