// #include "shaders.hpp"
#include "gol.hpp"
#include <thread>
// #include <vector>
// #include "square.hpp"
//
// #include <glad/glad.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

glm::vec3 red = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 green = glm::vec3(0.f, 1.f, 0.f);
glm::vec3 blue = glm::vec3(0.f, 0.f, 1.f);
glm::vec3 grey = glm::vec3(0.3f, 0.3f, 0.3f);

// Gol::Gol(Square square, Shader shader_program, int win_size)
Gol::Gol(Shader shader_program, int win_size)
    : shader_program(shader_program), win_size(win_size),
        cell_size(SQUARE_SIZE + GAP), rows(win_size / cell_size), cols(rows),
            cells(rows, std::vector<int>(cols, 0)), seeding(true) {


    square = new Square(shader_program, SQUARE_SIZE);
    origin_x = -1.f + pxls_to_float(cell_size) / 2;
    origin_y = 1.f - pxls_to_float(cell_size) / 2;
}

void Gol::init() {
}

bool Gol::inbounds(int row, int col) {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void Gol::set_value(double x_pos, double y_pos, int val) {
    int row = (int) x_pos / cell_size;
    int col = (int) y_pos / cell_size;

    if(inbounds(row, col) && !cells[row][col]) {
        cells[row][col] = val;
    }
}

float Gol::pxls_to_float(int pixels) {
    return 2 * float(pixels) / win_size;
}

// https://en.wikipedia.org/wiki/Conways_Game_of_Life
void Gol::apply_rules(int row, int col) {
    int neighbors = 0;
    bool alive = cells[row][col] ? true : false;

    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; j++) {
            if(i == 0 && j == 0) continue;

            if(inbounds(row + i, col + j)
                && cells[row + i][col + j]){
                neighbors++;
            }
        }
    }
    // std::cout << "row: " << row << " | col: " << col << "neigh" << neighbors<< std::endl;

    // 1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
    if(alive && neighbors < 2){cells[row][col] = 0; return; } 
    // 2. Any live cell with two or three live neighbors lives on to the next generation.
    if(alive && (neighbors == 2 || neighbors == 3)) { cells[row][col] = 1; return; }
    // 3. Any live cell with more than three live neighbors dies, as if by overpopulation.
    if(alive && neighbors > 3) { cells[row][col] = 0; return; }
    // 4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
    if(!alive && neighbors == 3) { cells[row][col] = 1; return; }

}

void Gol::draw() {
    glm::mat4 model;
    counter++;

    auto start_time = std::chrono::high_resolution_clock::now();
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
                // std::cout << counter << std::endl;
            model = glm::mat4(1.0f);

            float x = origin_x + pxls_to_float(cell_size) * row;
            float y = origin_y - pxls_to_float(cell_size) * col;

            model = glm::translate(model, glm::vec3(x, y, 0.f));

            shader_program.set_mat4("model", model);

                // std::cout << counter << " | " << row << " | " << col << std::endl;
            if(counter >= delay && !seeding) {
                apply_rules(row, col);
                // std::cout << counter << " | " << row << " | " << col << std::endl;
            }

            cells[row][col]
                ? square->set_color(green)
                : square->set_color(grey);

            square->draw(GL_TRIANGLES);
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
        }
    }
            if(counter >= delay) {
                counter = 0;}
}
