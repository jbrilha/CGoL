#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "shaders.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Square {
    static const int INDICES_NR = 6;
    const glm::vec3 DEFAULT_COLOR = glm::vec3(0.f, 1.f, 0.f); // white

  public:
    Square(Shader program, float size);
    Square(Shader program, float size, glm::vec3 color);
    void draw(unsigned int primitive);
    void set_color(glm::vec3 color);

  private:
    // void init(glm::vec3 color);
    void init();
    const float size;
    Shader shader_program;

    bool color_set;
    unsigned int VBO, VAO, EBO; // vertex buffer object, vertex array

    glm::vec3 color;
    glm::vec3 position;
};
#endif
