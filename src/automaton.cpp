#include "automaton.hpp"

Automaton::Automaton(std::string path_str, int win_width, int win_height, int square_size)
    : win_height(win_height), win_width(win_width), square_size(square_size),
      cell_size(square_size + GAP),
      cell_count((win_width / cell_size) * (win_height / cell_size)),
      rows(win_height / cell_size), cols(win_width / cell_size), plague(false),
      cells(cell_count, 0), update_cells(cells),
      shader_program((path_str + "/shaders/shader.vert").c_str(),
                     (path_str + "/shaders/shader.frag").c_str()) {

    float row_rem = win_height % cell_size;
    float col_rem = win_width % cell_size;
    this->origin_x =
        -1.f + pxls_to_float(this->cell_size + col_rem, win_width) / 2;
    this->origin_y =
        1.f - pxls_to_float(this->cell_size + row_rem, win_height) / 2;

    prepare_shaders();
}

void Automaton::update_grid() {
    rows = win_height / cell_size;
    cols = win_width / cell_size;

    int row_rem = win_height % cell_size;
    int col_rem = win_width % cell_size;

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

    origin_x = -1.f + pxls_to_float(cell_size + col_rem, win_width) / 2;
    origin_y = 1.f - pxls_to_float(cell_size + row_rem, win_height) / 2;

    prepare_shaders();
}

void Automaton::update_dimensions(int win_width, int win_height) {
    this->win_width = win_width;
    this->win_height = win_height;

    update_grid();
}

void Automaton::update_cell_size(int val) {
    bool update = false;

    if (val > 0) {
        square_size += val;
        update = true;
    } else if (square_size > 1) {
        square_size = std::max(1, square_size + val);
        update = true;
    }

    if (update) {
        cell_size = square_size + GAP;
        update_grid();
    }
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

    glGenBuffers(2, instanceVBOs);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * (cell_count), &offsets[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * (cell_count), &cells[0],
                 GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float x_vert = pxls_to_float(square_size, win_width) / 2;
    float y_vert = pxls_to_float(square_size, win_height) / 2;
    float quadVertices[12] = {
        -x_vert, y_vert,  // top right
        x_vert,  -y_vert, // bot right
        -x_vert, -y_vert, // bot left

        -x_vert, y_vert, // top left
        x_vert,  -y_vert, x_vert, y_vert,
    };

    unsigned int quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);

    // also set instance data
    glEnableVertexAttribArray(1);
    glBindBuffer(
        GL_ARRAY_BUFFER,
        instanceVBOs[0]); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(
        1, 1); // tell OpenGL this is an instanced vertex attribute.

    glEnableVertexAttribArray(2);
    glBindBuffer(
        GL_ARRAY_BUFFER,
        instanceVBOs[1]); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, 1 * sizeof(int), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(
        2, 1); // tell OpenGL this is an instanced vertex attribute.
}

void Automaton::update_states() {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * (cell_count), &cells[0],
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Automaton::set_value(double x_pos, double y_pos, int val) {
    int col = floor(((int)x_pos - GAP) % (int)(win_width)) / cell_size;
    int row = floor(((int)y_pos - 2 * GAP) % (int)(win_height)) / cell_size;
    int offset = row * cols + col;

    if (offset >= 0 && offset < cell_count) {
        cells[offset] = val;
        update_states();
    }
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
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, cell_count);
    glBindVertexArray(0);
}

void Automaton::clear() {
    cells.clear();
    update_cells.clear();
    cells = std::vector<int>(cell_count, 0);
    update_cells = std::vector<int>(cell_count, 0);

    update_states();
}

void Automaton::toggle_plague() { plague = !plague; }
int Automaton::get_cell_count() { return cell_count; }
int Automaton::get_square_size() { return square_size; }

float Automaton::pxls_to_float(int pixels, int total_pixels) {
    return 2 * (float(pixels) / total_pixels);
}
