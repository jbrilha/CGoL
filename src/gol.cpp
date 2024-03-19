#include "gol.hpp"
#include "glm/fwd.hpp"
#include <algorithm>

glm::vec3 red = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 green = glm::vec3(0.f, 1.f, 0.f);
glm::vec3 blue = glm::vec3(0.f, 0.f, 1.f);
glm::vec3 grey = glm::vec3(0.3f, 0.3f, 0.3f);

Gol::Gol(Shader shader_program, int win_width, int win_height)
    : shader_program(shader_program), win_height(win_height), win_width(win_width),
      cell_size(SQUARE_SIZE + GAP), rows(win_height / cell_size), cols(win_width / cell_size),
      cells(rows, std::vector<int>(cols, 0)), states_vec(rows * cols, 0), update_cells(cells) {

    // square = new Square(shader_program, pxls_to_float(SQUARE_SIZE, std::min(win_height, win_width)));

    origin_x = -1.f + pxls_to_float(cell_size, win_width) / 2;
    origin_y = 1.f - pxls_to_float(cell_size, win_height) / 2;
    prepare_shaders();
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

void Gol::prepare_shaders() {
    glm::vec2 offsets[rows * cols];
    glm::vec2 states[rows * cols];
    int i = 0;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            glm::vec3 offset;
            glm::vec2 state;

            offset.x = origin_x + pxls_to_float(cell_size, win_width) * col;
            offset.y = origin_y - pxls_to_float(cell_size, win_height) * row;
            // state.x = (!(row % 2) || !(col % 2)) ?  0 :  1;
            state.x = 0;
            state.y = 0;

            states[i] = state;
            offsets[i] = offset;
            // std::cout << i << " || " << offsets[i].x << " / " << offsets[i].y << " || sz: " << offsets->length() << std::endl;
            // std::cout << i << " || " << states[i].x << " / " << states[i].y << " || sz: " << states->length() << std::endl;
            i++;
        }
    }

    std::cout << "instanced!" << std::endl;

    glGenBuffers(2, instanceVBOs);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * (rows * cols), &offsets[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * (rows * cols), &states[0], GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    static const float vert = pxls_to_float(SQUARE_SIZE, std::min(win_height, win_width)) / 2;
    float quadVertices[12] = {
       -vert,  vert,  // top right
        vert, -vert,  // bot right
       -vert, -vert,  // bot left

       -vert,  vert,  // top left
        vert, -vert,
        vert,  vert,
    };

    unsigned int quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)(0 * sizeof(float)));

    // also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[0]); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[1]); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.
}

void Gol::update_states() {
    glm::vec2 states[rows * cols];
    int i = 0;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            glm::vec2 state;

            state.x = cells[row][col] ? 1 : 0;
            // state.x = (!(row % 2) || !(col % 2)) ?  0 :  1;
            state.y = 0;

            states[i] = state;
            // std::cout << i << " || " << states[i].x << " / " << states[i].y << " || sz: " << states->length() << std::endl;
            i++;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * (rows * cols), &states[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glEnableVertexAttribArray(3);
    // glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[1]); // this attribute comes from a different vertex buffer
    // glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.
}

void Gol::update() {
    update_cells = cells;
    int alive = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            glm::vec2 state;
            alive = cells[row][col] ? 1 : 0;

            int neighbors = apply_rules(row, col);

            if (alive && (neighbors < 2 || neighbors > 3)){
                // std::cout << "Alive!" << std::endl;
                update_cells[row][col] = 0;
            } 
            if (!alive && neighbors == 3) {
                update_cells[row][col] = 1;
            }
        }
    }
    cells = update_cells;
    update_states();
}

bool Gol::inbounds(int row, int col) {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void Gol::set_value(double x_pos, double y_pos, int val) {
    int col = floor(((int)x_pos - GAP) % (int)(win_width)) / cell_size;
    int row = floor(((int)y_pos - 2 * GAP) % (int)(win_height)) / cell_size;

    // std::cout << "\r"
    //     << "r: " << row << "/" << rows
    //     << " | c: " << col << "/" << cols
    //     << " || x: " << x_pos << " | y: " << y_pos
    //     << " || w: " << win_width << " | h: " << win_height << "" 
    //     << std::flush;

    if (inbounds(row, col)) {
        cells[row][col] = val;
    }

    update_states();
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

// void Gol::render_cell(bool alive, int row, int col, glm::vec3 color1,
//                       glm::vec3 color2) {
//     float x = origin_x + pxls_to_float(cell_size, win_width) * col;
//     float y = origin_y - pxls_to_float(cell_size, win_height) * row;
//
//     glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));
//
//     float aspect_ratio = win_height / win_width;
//     if(aspect_ratio < 1.f) {
//         model = glm::scale(model, glm::vec3(aspect_ratio, 1.f, 0.f));
//     }
//     else {
//         model = glm::scale(model, glm::vec3(1.f, 1/aspect_ratio, 0.f));
//     }
//
//     shader_program.set_mat4("model", model);
//
//     alive ? square->set_color(color1) : square->set_color(color2);
//
//     square->draw(GL_TRIANGLES);
// }

void Gol::draw() {
    shader_program.use();
    glBindVertexArray(quadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, rows*cols); // 100 triangles of 6 vertices each
    glBindVertexArray(0);
}
    // for (int row = 0; row < rows; row++) {
    //     for (int col = 0; col < cols; col++) {
    //         bool alive = cells[row][col];
    //         // if(alive)
    //             render_cell(alive, row, col, green, grey);
    //     }
    // }
