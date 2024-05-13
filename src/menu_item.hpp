#ifndef MENU_ITEM_HPP
#define MENU_ITEM_HPP

#include "shaders.hpp"
#include "util/glm_colors.hpp"
#include "util/pxls.hpp"

#include <vector>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm_colors;

class MenuItem {
  private:
    virtual void set_vertices() = 0;
    virtual void set_model() = 0;
    void set_color();

  public:
    MenuItem();
    MenuItem(std::string path_str, GLFWwindow *window, glm::vec3 position);
    MenuItem(std::string path_str, GLFWwindow *window, glm::vec3 position, glm::vec3 color);
    virtual ~MenuItem();

    virtual void draw();
    void update_position(double x_pos, double y_pos);
    void update_dimensions(int win_width, int win_height);
    void change_shape();

    int get_radius();
    virtual void click() = 0;

  protected:
    unsigned int quad_VAO;
    int square_size;
    void set_shaders();
    Shader shader_program;
    glm::mat4 model;
    std::vector<glm::vec2> quad_vertices;

    glm::vec3 position;
    glm::vec3 color;

    int win_height;
    int win_width;

    GLFWwindow *window;
};
#endif
