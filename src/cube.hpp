#ifndef CUBE_H
#define CUBE_H

#include "shaders.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cube {
  public:
    Cube() {
        init();
    }
    void draw(Shader shader_program, unsigned int primitive) {
        shader_program.use();



        glBindVertexArray(VAO);
        glDrawElements(primitive, sizeof(indices), GL_UNSIGNED_INT, 0);
        // glDrawElements(primitive, primitive == GL_LINES ? 12: 36, GL_UNSIGNED_INT, 0);
    }

  private:
    unsigned int VBO, VAO, EBO; // vertex buffer object, vertex array
                                // object, element buffer object

    int indices[36] = {
        // top
        0, 3, 7,
        0, 7, 4,
        // bot
        1, 5, 6,
        1, 6, 2,
        // left
        3, 2, 6,
        3, 6, 7,
        // right
        0, 4, 5,
        0, 5, 1,
        // back
        0, 1, 2,
        0, 2, 3,
        // front
        4, 7, 6,
        4, 6, 5,
    };

    float vertices[24] = {
         0.5f,  0.5f, -0.5f,   // back top right
         0.5f, -0.5f, -0.5f,  // back bot right
        -0.5f, -0.5f, -0.5f, // back bot left
        -0.5f,  0.5f, -0.5f,  // back top left
                             //
         0.5f,  0.5f,  0.5f,    // front top right
         0.5f, -0.5f,  0.5f,   // front bot right
        -0.5f, -0.5f,  0.5f,  // front bot left
        -0.5f,  0.5f,  0.5f,   // front top left
    };

    // float normals[18] {
    //       0.f,  0.f, -1.f,
    //       0.f,  0.f,  1.f,
    //      -1.f,  0.f,  0.f,
    //       1.f,  0.f,  0.f,
    //       0.f, -1.f,  0.f,
    //       0.f,  1.f,  0.f,
    // };

    void init() {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // glGenBuffers(1, &normal_buffer);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        // glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals,
        //              GL_STATIC_DRAW);

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
};
#endif
