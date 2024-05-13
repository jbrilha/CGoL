#ifndef BUTTON_MENU_ITEM_HPP
#define BUTTON_MENU_ITEM_HPP

#include "src/menu_item.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm_colors;

class Button : public MenuItem {
  private:
    void set_vertices() override;
    void set_model() override;

    unsigned int quad_VAO;
    unsigned int cursor_VBO;

    int height;
    int width;

  public:
    Button();
    Button(std::string path_str, GLFWwindow *window, int width, int height, glm::vec3 position);
    Button(std::string path_str, GLFWwindow *window, int width, int height, glm::vec3 position, glm::vec3 color);
    ~Button();

    void click() override;

    bool clicked;
};
#endif
