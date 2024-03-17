// #include "shaders.hpp"
#include "square.hpp"
//
// #include <glad/glad.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

Square::Square(Shader shader_program, float size) : shader_program(shader_program), size(size), color_set(false) {
    init();
    // init(DEFAULT_COLOR);
}
Square::Square(Shader shader_program, float size, glm::vec3 color) : shader_program(shader_program), size(size), color_set(true), color(color) {
    // init(color);
    init();
}

void Square::set_color(glm::vec3 color) {
    color_set = true;
    this->color = color;
}

void Square::draw(unsigned int primitive) {
    shader_program.use();
    if(color_set)
        shader_program.set_vec3("uColor", color);

    glBindVertexArray(VAO);
    glDrawElements(primitive, 6, GL_UNSIGNED_INT, 0);
}

void Square::init() {
    // set_color(color);

    static const unsigned int indices[INDICES_NR] = {
        0, 1, 3,
        1, 2, 3
    };

    static const float vert = size / 2;
    static float vertices[12] = {
        vert,  vert, 0.f,  // top right
        vert, -vert, 0.f,  // bot right
       -vert, -vert, 0.f,  // bot left
       -vert,  vert, 0.f,  // top left
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
                          (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}
