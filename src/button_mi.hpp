#ifndef BUTTON_MENU_ITEM_HPP
#define BUTTON_MENU_ITEM_HPP

#include "shaders.hpp"
#include "constants.hpp"
#include "src/menu_item.hpp"
#include "util/glm_colors.hpp"
#include "util/pxls.hpp"

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm_colors;

class Button : public MenuItem {
  private:
    glm::vec3 center;
    glm::vec3 offset;
    void set_vertices() override;
    void set_model() override;

    unsigned int quad_VAO;
    unsigned int cursor_VBO;

  public:
    Button();
    Button(std::string path_str, int win_width, int win_height, int square_size, int radius);
    ~Button();

    // void draw();

    void click() override;

    bool clicked;
};
#endif
