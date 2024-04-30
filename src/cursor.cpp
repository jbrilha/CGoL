#include "cursor.hpp"

Cursor::Cursor(std::string path_str, int win_width, int win_height, int square_size, int radius)
    : square_size(square_size), win_height(win_height), win_width(win_width),
      radius(radius), cursor_x(0), cursor_y(0), cursor_pos(0),
      shader_program((path_str + "/../shaders/cursor_vert.glsl").c_str(),
                     (path_str + "/../shaders/cursor_frag.glsl").c_str()) {

    prepare_shaders();
}

Cursor::~Cursor() { glDeleteProgram(shader_program.program_ID); }

void Cursor::prepare_shaders() {
    shader_program.use();

    int dimensions =
        radius ? (square_size + GAP) * (radius * 2 + 1) : square_size + GAP;

    float x_vert = pxls::to_float(dimensions, win_width);
    float y_vert = pxls::to_float(dimensions, win_height);
    glm::vec2 quad_vertices[4] = {
        glm::vec2(x_vert, y_vert),
        glm::vec2(x_vert, -y_vert),
        glm::vec2(-x_vert, -y_vert),
        glm::vec2(-x_vert, y_vert),
    };

    unsigned int quad_VBO;
    glGenVertexArrays(1, &quad_VAO);
    glGenBuffers(1, &quad_VBO);
    glBindVertexArray(quad_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
}

void Cursor::update_position(double x_pos, double y_pos) {
    cursor_pos = glm::vec2(2 * pxls::to_float(x_pos, win_width) - 1.f,
                           -2 * pxls::to_float(y_pos, win_height) + 1.f);

    shader_program.set_vec2("cursor_pos", cursor_pos);
}

void Cursor::update_square_size(int square_size) {
    this->square_size = square_size;

    prepare_shaders();
}

void Cursor::update_dimensions(int win_width, int win_height) {
    this->win_width = win_width;
    this->win_height = win_height;

    prepare_shaders();
}

void Cursor::update_radius(int radius) {
    this->radius = radius;

    prepare_shaders();
}

void Cursor::draw() {
    shader_program.use();

    glBindVertexArray(quad_VAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);
}

int Cursor::get_radius() { return radius; }
