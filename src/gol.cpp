#include "gol.hpp"
#include "shaders.hpp"
#include <algorithm>
//
// glm::vec3 red = glm::vec3(1.f, 0.f, 0.f);
// glm::vec3 green = glm::vec3(0.f, 1.f, 0.f);
// glm::vec3 blue = glm::vec3(0.f, 0.f, 1.f);
// glm::vec3 grey = glm::vec3(0.3f, 0.3f, 0.3f);

Gol::Gol() : cell_size(SQUARE_SIZE + GAP) {}

Gol::Gol(std::string path_str, int win_width, int win_height)
    : shader_program((path_str + "/shaders/shader.vert").c_str(),
                     (path_str + "/shaders/shader.frag").c_str()),
      win_height(win_height), win_width(win_width),
      cell_size(SQUARE_SIZE + GAP), rows(win_height / cell_size),
      cols(win_width / cell_size),
      cell_count((win_width / cell_size) * (win_height / cell_size)),
      plague(false), cells(cell_count, 0), update_cells(cells) {

    origin_x = -1.f + pxls_to_float(cell_size, win_width) / 2;
    origin_y = 1.f - pxls_to_float(cell_size, win_height) / 2;
    prepare_shaders();
}

void Gol::update_dimensions(int win_width, int win_height) {
    this->win_width = win_width;
    this->win_height = win_height;

    rows = win_height / cell_size;
    cols = win_width / cell_size;

    int new_cell_count = (win_width / cell_size) * (win_height / cell_size);
    std::vector<int>new_cells (new_cell_count, 0);
    if(new_cell_count > cell_count) {
        int offset = (new_cell_count - cell_count) / 2;

        std::copy(cells.begin(), cells.end(), new_cells.begin() + offset);

        // cells = std::vector<int>(new_cell_count, 0);
        cells = new_cells;
    }
    else {
        std::copy(cells.begin(), cells.begin() + new_cell_count, new_cells.begin());
        // cells = std::vector<int>(new_cell_count, 0);
        cells = new_cells;
        // cells.shrink_to_fit();
    }
    cell_count = new_cell_count;
    update_cells = std::vector<int>(cell_count, 0);

    origin_x = -1.f + pxls_to_float(cell_size, win_width) / 2;
    origin_y = 1.f - pxls_to_float(cell_size, win_height) / 2;

    prepare_shaders();
}

void Gol::prepare_shaders() {
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

    float x_vert = pxls_to_float(SQUARE_SIZE, win_width) / 2;
    float y_vert = pxls_to_float(SQUARE_SIZE, win_height) / 2;
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

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(
        GL_ARRAY_BUFFER,
        instanceVBOs[0]); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(
        2, 1); // tell OpenGL this is an instanced vertex attribute.

    glEnableVertexAttribArray(3);
    glBindBuffer(
        GL_ARRAY_BUFFER,
        instanceVBOs[1]); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, 1 * sizeof(int), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(
        3, 1); // tell OpenGL this is an instanced vertex attribute.
}

void Gol::update_states() {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * (cell_count), &cells[0],
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Gol::update() {
    char alive = 0;

    for (int offset = 0; offset < cell_count; offset++) {
        alive = cells[offset];

        char neighbors = apply_rules(offset);

        if (alive && (neighbors < 2 || neighbors > 3)) {
            if (!plague)
                update_cells[offset] = 0;
            else
                cells[offset] = 0;
        } else if (!alive && neighbors == 3) {
            if (!plague)
                update_cells[offset] = 1;
            else
                cells[offset] = 1;
        } else {
            if (!plague)
                update_cells[offset] = alive;
            else
                cells[offset] = alive;
        }
    }
    if (!plague)
        cells = update_cells;

    update_states();
}

void Gol::set_value(double x_pos, double y_pos, int val) {
    int col = floor(((int)x_pos - GAP) % (int)(win_width)) / cell_size;
    int row = floor(((int)y_pos - 2 * GAP) % (int)(win_height)) / cell_size;
    int offset = row * cols + col;

    if (offset >= 0 && offset < cell_count) {
        cells[offset] = val;
        update_states();
    }
}

void Gol::fill_random() {
    for(int &cell : cells) {
        cell = (rand() % 100) < 25 ? 1 : 0;
    }
    update_states();
}

char Gol::apply_rules(int offset) {
    char neighbors = 0;
    int row = offset / cols;
    int col = offset % cols;

    for (char i = -1; i < 2; i++) {
        for (char j = -1; j < 2; j++) {
            if (i == 0 && j == 0)
                continue;

            int neigh_row = (row + i + rows) % rows;
            int neigh_col = (col + j + cols) % cols;
            int offset = neigh_row * cols + neigh_col;

            if (offset >= 0 && offset < cell_count && cells[offset]) {
                neighbors++;
            }
        }
    }

    return neighbors;
}

void Gol::draw() {
    shader_program.use();

    glBindVertexArray(quadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, cell_count);
    glBindVertexArray(0);
}

void Gol::clear() {
    cells.clear();
    cells = std::vector<int>(cell_count, 0);
    update_states();
}

void Gol::toggle_plague() { plague = !plague; }

float Gol::pxls_to_float(int pixels, int total_pixels) {
    return 2 * (float(pixels) / total_pixels);
}
