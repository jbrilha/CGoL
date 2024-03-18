#include "gol.hpp"

glm::vec3 red = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 green = glm::vec3(0.f, 1.f, 0.f);
glm::vec3 blue = glm::vec3(0.f, 0.f, 1.f);
glm::vec3 grey = glm::vec3(0.3f, 0.3f, 0.3f);

Gol::Gol(Shader shader_program, int win_size)
    : shader_program(shader_program), win_size(win_size),
      cell_size(SQUARE_SIZE + GAP), rows(win_size / cell_size), cols(rows),
      cells(rows, std::vector<int>(cols, 0)),
      update_cells(cells) {

    square = new Square(shader_program, pxls_to_float(SQUARE_SIZE));
    origin_x = -1.f + pxls_to_float(cell_size) / 2;
    origin_y = 1.f - pxls_to_float(cell_size) / 2;
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
    int row = (int)x_pos / cell_size;
    int col = (int)y_pos / cell_size;

    if (inbounds(row, col)) {
        cells[row][col] = !cells[row][col];
    }
}

void Gol::set_value(double x_pos, double y_pos, int val) {
    int row = (int)x_pos / cell_size;
    int col = (int)y_pos / cell_size;

    if (inbounds(row, col)) {
        cells[row][col] = val;
    }
}

float Gol::pxls_to_float(int pixels) { return 2 * float(pixels) / win_size; }

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
    float x = origin_x + pxls_to_float(cell_size) * row;
    float y = origin_y - pxls_to_float(cell_size) * col;

    glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));

    shader_program.set_mat4("model", model);

    alive ? square->set_color(color1) : square->set_color(color2);

    square->draw(GL_TRIANGLES);
}

void Gol::draw() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            bool alive = cells[row][col];
            render_cell(alive, row, col, green, grey);
        }
    }
}
