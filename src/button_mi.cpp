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
        glm::vec2(x_vert, y_vert),
        glm::vec2(x_vert, -y_vert),
        glm::vec2(-x_vert, -y_vert),
        glm::vec2(-x_vert, y_vert),
    };
}

void Button::click() { std::cout << "TODO" << std::endl; }
