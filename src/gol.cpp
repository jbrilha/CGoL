#include "gol.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <algorithm>

glm::vec3 red = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 green = glm::vec3(0.f, 1.f, 0.f);
glm::vec3 blue = glm::vec3(0.f, 0.f, 1.f);
glm::vec3 grey = glm::vec3(0.3f, 0.3f, 0.3f);

// Gol::Gol(Shader shader_program, int win_size)
//     : shader_program(shader_program), win_size(win_size),
Gol::Gol(Shader shader_program, int win_width, int win_height)
    : shader_program(shader_program), win_height(win_height), win_width(win_width), // win_size(win_width / win_height),
      /*cell_size(SQUARE_SIZE + GAP),*/cell_size(SQUARE_SIZE + GAP), rows(win_height / cell_size), cols(win_width / cell_size),
      cells(rows, std::vector<int>(cols, 0)),
      update_cells(cells) {

    // square = new Square(shader_program, pxls_to_float(SQUARE_SIZE - GAP, std::min(win_height - GAP, win_width- GAP)));
    square = new Square(shader_program, pxls_to_float(SQUARE_SIZE, std::min(win_height, win_width)));
    // float pxl_offset = pxls_to_float(cell_size, std::max(win_width, win_height));
    // origin_x = -1.f + pxl_offset / 2;
    // origin_y = 1.f - pxl_offset / 2;
    origin_x = -1.f + pxls_to_float(cell_size, win_width) / 2;
    origin_y = 1.f - pxls_to_float(cell_size, win_height) / 2;
}

void Gol::update_dimensions(int win_width, int win_height) {
    this->win_width = win_width;
    this->win_height = win_height;
    rows = win_height / cell_size;
    cols = win_width / cell_size;
    // square = nullptr;
    // TODO: update cells vector otherwise it crashes when trying to size window up

    // square = new Square(shader_program, pxls_to_float(SQUARE_SIZE - GAP, std::min(win_height - GAP, win_width- GAP)));
}

void Gol::update() {
    update_cells = cells;

    bool alive = false;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {

            alive = cells[row][col] ? true : false;

            int neighbors = apply_rules(row, col);

            // 1. Any live cell with fewer than two live neighbors dies, as if
            // by underpopulation.
            // 2. Any live cell with two or three live neighbors lives on to the
            // next generation.
            // 3. Any live cell with more than three live neighbors dies, as if
            // by overpopulation.
            // 4. Any dead cell with exactly three live neighbors becomes a live
            // cell, as if by reproduction.

            if (alive && (neighbors < 2 || neighbors > 3)) update_cells[row][col] = 0;
            if (!alive && neighbors == 3) update_cells[row][col] = 1;
        }
    }
    cells = update_cells;
}

bool Gol::inbounds(int row, int col) {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void Gol::toggle_value(double x_pos, double y_pos) {
    // int row = (int)x_pos / (cell_size * (win_height / win_width));
    int row = (int)y_pos / (cell_size);
    int col = (int)x_pos / (cell_size * (win_height / win_width));


    if (inbounds(row, col)) {
        cells[row][col] = !cells[row][col];
    }
}

void Gol::set_value(double x_pos, double y_pos, int val) {
    // int row = floor(((int)y_pos - GAP) % (int)(win_height - GAP)) / cell_size;
    // int col = floor(((int)x_pos - GAP) % (int)(win_width - GAP)) / cell_size;
    int col = floor(((int)x_pos - GAP) % (int)(win_width)) / cell_size;
    int row = floor(((int)y_pos - 2 * GAP) % (int)(win_height)) / cell_size;
    // int row = floor((int)y_pos % win_height) / cell_size;
    // int col = floor((int)x_pos % win_width) / cell_size;
    // int row = (int)y_pos / cell_size;
    // int col = (int)x_pos / cell_size;

    std::cout << "\r"
        << "r: " << row << "/" << rows
        << " | c: " << col << "/" << cols
        << " || x: " << x_pos << " | y: " << y_pos
        << " || w: " << win_width << " | h: " << win_height
        << std::flush;
        // << std::endl; 

    if (inbounds(row, col)) {
        cells[row][col] = val;
    }
}

float Gol::pxls_to_float(int pixels, int total_pixels) { return 2 * (float(pixels) / total_pixels); }

int Gol::apply_rules(int row, int col) {
    int neighbors = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) continue;

            int neigh_row = (row + i + rows) % rows;
            int neigh_col = (col + j + cols) % cols;

            if (inbounds(neigh_row, neigh_col) && cells[neigh_row][neigh_col]) {
                neighbors++;
            }
        }
    }

    return neighbors;
}

void Gol::render_cell(bool alive, int row, int col, glm::vec3 color1,
                      glm::vec3 color2) {
    // float x = origin_x + pxls_to_float(cell_size) * row;
    // float pxl_offset = pxls_to_float(cell_size, std::min(win_width, win_height));
    // float x = origin_x + pxl_offset * col;
    // float y = origin_y - (pxl_offset) * ((float)win_width / (float)win_height) * row;

    float x = origin_x + pxls_to_float(cell_size, win_width) * col;
    float y = origin_y - pxls_to_float(cell_size, win_height) * row;

    glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));

    float aspect_ratio = win_height / win_width;
    if(aspect_ratio < 1.f) {
        model = glm::scale(model, glm::vec3(aspect_ratio, 1.f, 0.f));
    }
    else {
        model = glm::scale(model, glm::vec3(1.f, 1/aspect_ratio, 0.f));
    }
    // model = glm::scale(model, glm::vec3(1.f * ((float)win_width / (float)win_height), 1.f, 0.f));

    shader_program.set_mat4("model", model);

    alive ? square->set_color(color1) : square->set_color(color2);

    square->draw(GL_TRIANGLES);
}

void Gol::draw() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            bool alive = cells[row][col];
            // if(alive) std::cout << "alive @ " << row << ":" << col << std::endl;
            render_cell(alive, row, col, green, grey);
        }
    }
}
