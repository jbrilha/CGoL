#include "automaton.hpp"
#include <iomanip>
#include <string>

Automaton::Automaton(std::string path_str, int win_width, int win_height, int square_size)
    : square_size(square_size), cell_size(square_size + GAP), win_height(win_height),
      win_width(win_width), hei_margin(win_height % cell_size), wid_margin(win_width % cell_size),
      cell_count((win_width / cell_size) * (win_height / cell_size)),
      rows(win_height / cell_size), cols(win_width / cell_size), plague(false),
      cells(cell_count, 0), update_cells(cells),
      shader_program((path_str + "/../shaders/vert.glsl").c_str(),
                     (path_str + "/../shaders/frag.glsl").c_str()) {

    this->origin_x =
        -1.f + pxls_to_float(this->cell_size + wid_margin, win_width) / 2;
    this->origin_y =
        1.f - pxls_to_float(this->cell_size + hei_margin, win_height) / 2;

    colors.push_back(black);
    prepare_shaders();
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

    origin_x = -1.f + pxls_to_float(cell_size + wid_margin, win_width) / 2;
    origin_y = 1.f - pxls_to_float(cell_size + hei_margin, win_height) / 2;

    prepare_shaders();
}

void Automaton::update_dimensions(int win_width, int win_height) {
    this->win_width = win_width;
    this->win_height = win_height;

    update_grid();
}

void Automaton::update_cell_size(int val) {
    square_size = std::max(1, square_size + val);
    cell_size = square_size + GAP;

    update_grid();
}

void Automaton::prepare_shaders() {
    shader_program.use();

    glm::vec2 offsets[cell_count];
    int i = 0;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            offsets[i++] = {
                origin_x + pxls_to_float(cell_size, win_width) * col,
                origin_y - pxls_to_float(cell_size, win_height) * row};
        }
    }

    glGenBuffers(2, instance_VBOs);
    glBindBuffer(GL_ARRAY_BUFFER, instance_VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * (cell_count), &offsets[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, instance_VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * (cell_count), &cells[0],
                 GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float x_vert = pxls_to_float(square_size, win_width) / 2;
    float y_vert = pxls_to_float(square_size, win_height) / 2;
    glm::vec2 quad_vertices[4] = {
        glm::vec2(x_vert, y_vert),
        glm::vec2(x_vert, -y_vert),
        glm::vec2(-x_vert, -y_vert),
        glm::vec2(-x_vert, y_vert),
    };

    unsigned int quad_VBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quad_VBO);
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices,
                 GL_STATIC_DRAW);

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
    glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, 1 * sizeof(int), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(
        2, 1); // tell OpenGL this is an instanced vertex attribute.
}

void Automaton::update_states() {
    glBindBuffer(GL_ARRAY_BUFFER, instance_VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * (cell_count), &cells[0],
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Automaton::set_value(double x_pos, double y_pos, int val, int radius) {
    bool update = false;
    // if(x_pos < (double) win_width / 2) x_pos -= 2.625;
    // if(y_pos < (double) win_height / 2) y_pos -= 3.234;

    if (x_pos < (double) wid_margin / 2 || x_pos > win_width - (double) wid_margin / 2 ||
        y_pos < (double) hei_margin / 2 || y_pos > win_height - (double)hei_margin / 2) {
        
        return;
    }
        // std::cout << "\r"
        // <<  "{" << x_pos << ", " << y_pos << "}" << std::setprecision(10) << std::fixed << "\n"
        // <<  wid_margin << "|" << hei_margin
        // << "[" << col << ":" << row << "]"
        // << std::flush;

    int col = (floor(((int)x_pos - GAP - wid_margin / 2) % (int)(win_width) )) / cell_size;
    int row = (floor(((int)y_pos - GAP - hei_margin / 2) % (int)(win_height))) / cell_size;
    int offset = row * cols + col;

    if (radius == 0) {
        cells[offset] = val;
        update_cells[offset] = val;
        update = true;
    } else {
        for (int i = row - radius; i < row + radius + 1; i++) {
            for (int j = col - radius; j < col + radius + 1; j++) {
                offset = i * cols + j;

                if (i >= 0 && i < rows && j < cols && j >= 0) {
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

    glBindVertexArray(quadVAO);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, cell_count);
    glBindVertexArray(0);
}

void Automaton::clear() {
    cells.clear();
    update_cells.clear();
    cells = std::vector<int>(cell_count, 0);
    update_cells = std::vector<int>(cell_count, 0);

    update_states();
}

void Automaton::set_cell_colors() {
    std::cout << "set cols" << std::endl; 
    for(int i = 0; i < colors.size(); i++) {
        std::cout << colors[i].x << colors[i].y << colors[i].z << std::endl;
        shader_program.set_vec3(("colors[" + std::to_string(i) + "]"), colors[i]);
    }
}

void Automaton::toggle_plague() { plague = !plague; }
int Automaton::get_cell_count() { return cell_count; }
int Automaton::get_square_size() { return square_size; }

float Automaton::pxls_to_float(int pixels, int total_pixels) {
    return 2 * (float(pixels) / total_pixels);
}
