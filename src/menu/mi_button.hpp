#ifndef BUTTON_MENU_ITEM_HPP
#define BUTTON_MENU_ITEM_HPP

#include "menu_item.hpp"
#include "utils/constants.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm_colors;

class Button : public MenuItem {
  private:
    void set_vertices() override;
    void set_circular_vertices() override;
    void set_model() override;

    unsigned int quad_VAO;
    unsigned int cursor_VBO;

    int height;
    int width;

  public:
    Button();
    Button(std::string path_str, GLFWwindow *window, int width, int height,
               glm::vec3 position, glm::vec3 color = white, int idx = -1, float angle = -1.f, bool circular = false);
    ~Button();

    void click() override;
    // void handle_cursor(double x_pos, double y_pos, bool clicking) override;

    bool clicked;
};
#endif
