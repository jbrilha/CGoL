#include "button_mi.hpp"
#include "util/pxls.hpp"

Button::Button(std::string path_str, GLFWwindow *window, int width, int height,
               glm::vec3 position)
    : MenuItem(path_str, window, position), height(height), width(width),
      clicked(false) {

    set_shaders();
}

Button::Button(std::string path_str, GLFWwindow *window, int width, int height,
               glm::vec3 position, glm::vec3 color)
    : MenuItem(path_str, window, position, color), height(height), width(width), clicked(false) {

    set_shaders();
}

Button::Button(std::string path_str, GLFWwindow *window, int width, int height,
               glm::vec3 position, glm::vec3 color, int idx)
    : MenuItem(path_str, window, position, color, idx), height(height), width(width), clicked(false) {

    set_shaders();
}

Button::Button(std::string path_str, GLFWwindow *window, int width, int height,
               glm::vec3 position, int idx)
    : MenuItem(path_str, window, position, idx), height(height), width(width), clicked(false) {

    set_shaders();
}

Button::~Button() { glDeleteProgram(shader_program.program_ID); }

void Button::set_model() {
    shader_program.use();

    model = glm::translate(model, position);
    shader_program.set_mat4("model", model);
}

void Button::set_vertices() {
    float x_vert = pxls::to_float(width, win_width);
    float y_vert = pxls::to_float(height, win_height);

    quad_vertices = {
        glm::vec2(x_vert, y_vert), //top right
        glm::vec2(x_vert, -y_vert), //bot right
        glm::vec2(-x_vert, -y_vert), // bot left
        glm::vec2(-x_vert, y_vert),
    };
}

// int Button::handle_cursor(double x_pos, double y_pos, bool clicking) {
//     glm::vec2 vert0 = quad_vertices[0] + glm::vec2(position.x, position.y); // top right
//     glm::vec2 vert2 = quad_vertices[2] + glm::vec2(position.x, position.y); // bot left
//
//     float v0_x = win_width * ((vert0.x) + 1.f) / 2;
//     float v0_y = win_height * (1.f - (vert0.y)) / 2;
//     float v2_x = win_width * ((vert2.x) + 1.f) / 2;
//     float v2_y = win_height * (1.f - (vert2.y)) / 2;
//
//     if(x_pos <= v0_x && y_pos >= v0_y
//         && x_pos >= v2_x && y_pos <= v2_y
//     ) {
//         if(clicking) {
//
//         }
//         else {
//             hover();
//         }
//     }
// }

void Button::click() { std::cout << "TODO" << std::endl; }
