#include "menu_item.hpp"

MenuItem::MenuItem(std::string path_str, int win_width, int win_height,
               int square_size, int radius)
    : square_size(square_size), win_height(win_height), win_width(win_width),
      radius(radius), circular(false), cursor_x(0), cursor_y(0), cursor_pos(0),
        model(1.f),
      shader_program((path_str + "/../shaders/menu_vert.glsl").c_str(),
                     (path_str + "/../shaders/menu_frag.glsl").c_str()) {

}

MenuItem::~MenuItem() { glDeleteProgram(shader_program.program_ID); }

void MenuItem::set_shaders() {
    std::cout << "mi ss" << std::endl;
    shader_program.use();

    set_vertices();
    set_model();

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

void MenuItem::update_position(double x_pos, double y_pos) {
    cursor_pos = glm::vec2(2 * pxls::to_float(x_pos, win_width) - 1.f,
                           -2 * pxls::to_float(y_pos, win_height) + 1.f);

    shader_program.set_vec2("cursor_pos", cursor_pos);
}

void MenuItem::update_square_size(int square_size) {
    this->square_size = square_size;

    set_shaders();
}

void MenuItem::update_dimensions(int win_width, int win_height) {
    this->win_width = win_width;
    this->win_height = win_height;

    set_shaders();
}

void MenuItem::update_radius(int radius) {
    this->radius = radius;

    set_shaders();
}

void MenuItem::draw() {
    std::cout << "mi d" << std::endl;
    shader_program.use();

    glBindVertexArray(quad_VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, quad_vertices.size());
    glBindVertexArray(0);
}

void MenuItem::change_shape() {
    circular = !circular;

    set_shaders();
}

int MenuItem::get_radius() { return radius; }
