#ifndef MENU_ITEM_HPP
#define MENU_ITEM_HPP

#include "shaders.hpp"
#include "constants.hpp"
#include "util/glm_colors.hpp"
#include "util/pxls.hpp"

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm_colors;

class MenuItem {
  private:
    int cell_size;

    int radius;
    
    bool circular;

    double cursor_x;
    double cursor_y;
    glm::vec2 cursor_pos;

    virtual void set_vertices() = 0;
    virtual void set_model() = 0;

    unsigned int cursor_VBO;

  public:
    MenuItem();
    MenuItem(std::string path_str, int win_width, int win_height, int square_size, int radius);
    virtual ~MenuItem();

    virtual void draw();
    void update_position(double x_pos, double y_pos);
    void update_radius(int radius);
    void update_dimensions(int win_width, int win_height);
    void update_square_size(int square_size);
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
    int win_height;
    int win_width;
};
#endif
