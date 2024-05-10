#include "button_mi.hpp"
#include "util/pxls.hpp"

Button::Button(std::string path_str, int win_width, int win_height,
                   int square_size, int radius)
    : MenuItem(path_str, win_width, win_height, square_size, radius),
      center(0.f), offset(0.f), clicked(false) {

    set_shaders();
}

Button::~Button() { glDeleteProgram(shader_program.program_ID); }

// void Button::set_shaders() {
//     shader_program.use();
//
//     set_vertices();
//     
//     offset = glm::vec3(pxls::to_float(win_width - OFFSET, win_width),
//                        pxls::to_float(win_height - OFFSET, win_height),
//                        0);
//
//     model = glm::mat4(1.0f);
//     model = glm::translate(model, center + offset);
//     shader_program.set_mat4("model", model);
//
//     unsigned int quad_VBO;
//     glGenVertexArrays(1, &quad_VAO);
//     glGenBuffers(1, &quad_VBO);
//     glBindVertexArray(quad_VAO);
//
//     glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
//     glBufferData(GL_ARRAY_BUFFER, quad_vertices.size() * sizeof(glm::vec2),
//                  quad_vertices.data(), GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
//                           (void *)0);
// }

void Button::set_model() {
    std::cout << "dd sm" << std::endl;
    shader_program.use();
    float nx = pxls::to_float(sin(glm::radians(30.f)) * square_size, win_width);
    float ny = pxls::to_float(cos(glm::radians(30.f)) * square_size, win_height);
    center = glm::vec3(nx, ny, 0.f);
    offset = glm::vec3(pxls::to_float(win_width - OFFSET, win_width),
                       pxls::to_float(win_height - OFFSET, win_height), 0);

    model = glm::translate(model, center - 0.5f);
    // model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
    shader_program.set_mat4("model", model);
}
void Button::set_vertices() {
    float x_vert = pxls::to_float(square_size * 6, win_width);
    float y_vert = pxls::to_float(square_size * 2, win_height);

    quad_vertices = {
        glm::vec2(x_vert, y_vert),
        glm::vec2(x_vert, -y_vert),
        glm::vec2(-x_vert, -y_vert),
        glm::vec2(-x_vert, y_vert),
    };
}

// void Button::collapse() {
//     shader_program.use();
//
//     // model = glm::mat4(1.0f);
//     // model = glm::translate(model, -center);
//     // model = glm::translate(model, offset);
//     model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));
//     // model = glm::translate(model, center);
//     shader_program.set_mat4("model", model);
// }

void Button::click() {

    std::cout << "TODO" << std::endl;
}

// void Button::set_circle_vertices() {
//     quad_vertices.clear();
//
//     int segments = radius < 60 ? 32 : 64;
//     float angle_step = 2.f * M_PI / segments;
//     int dimensions =
//         radius ? (square_size + GAP) * (radius * 2 + 1) : square_size + GAP;
//
//     for (int i = 0; i < segments; i++) {
//         float x_vert =
//             pxls::to_float(dimensions * cos(angle_step * i) + GAP, win_width);
//         float y_vert =
//             pxls::to_float(dimensions * sin(angle_step * i) + GAP, win_height);
//
//         quad_vertices.push_back(glm::vec2(x_vert, y_vert));
//     }
// }

