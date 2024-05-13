#include "menu_item.hpp"

MenuItem::MenuItem(std::string path_str, GLFWwindow *window, glm::vec3 position)
    : shader_program((path_str + "/../shaders/menu_vert.glsl").c_str(),
                     (path_str + "/../shaders/menu_frag.glsl").c_str()),
      model(1.f), position(position), color(1.f) {

    glfwGetWindowSize(window, &win_width, &win_height);
}

MenuItem::MenuItem(std::string path_str, GLFWwindow *window, glm::vec3 position, glm::vec3 color)
    : shader_program((path_str + "/../shaders/menu_vert.glsl").c_str(),
                     (path_str + "/../shaders/menu_frag.glsl").c_str()),
      model(1.f), position(position), color(color) {

    glfwGetWindowSize(window, &win_width, &win_height);
}

MenuItem::~MenuItem() { glDeleteProgram(shader_program.program_ID); }

void MenuItem::set_shaders() {
    std::cout << "mi ss" << std::endl;
    shader_program.use();

    set_vertices();
    set_model();
    set_color();

    unsigned int quad_VBO;
    glGenVertexArrays(1, &quad_VAO);
    glGenBuffers(1, &quad_VBO);
    glBindVertexArray(quad_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
    glBufferData(GL_ARRAY_BUFFER, quad_vertices.size() * sizeof(glm::vec2),
                 quad_vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
}

void MenuItem::set_color() {
    shader_program.set_vec3("aColor", color);
}

void MenuItem::update_position(double x_pos, double y_pos) {}

void MenuItem::update_dimensions(int win_width, int win_height) {
    this->win_width = win_width;
    this->win_height = win_height;

    set_shaders();
}

void MenuItem::draw() {
    // std::cout << "mi d" << std::endl;
    shader_program.use();

    glBindVertexArray(quad_VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, quad_vertices.size());
    glBindVertexArray(0);
}
