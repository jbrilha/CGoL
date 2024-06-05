#ifndef DROPDOWN_MENU_ITEM_HPP
#define DROPDOWN_MENU_ITEM_HPP

#include "shaders.hpp"
#include "utils/constants.hpp"
#include "menu_item.hpp"
#include "utils/glm_colors.hpp"
#include "utils/pxls.hpp"
#include "mi_button.hpp"

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm_colors;

class Dropdown : public MenuItem {
  private:
    glm::vec3 center;
    std::vector<MenuItem*> items;

    void set_vertices() override;
    void set_model() override;
    void init(std::string path_str);
    void collapse();
    void expand();

    bool collapsed;

    int size;

  public:
    Dropdown();
    Dropdown(std::string path_str, GLFWwindow *window, int dimensions, glm::vec3 position, glm::vec3 color = white);
    ~Dropdown();

    void draw() override;
    void click() override;
    int handle_cursor(double x_pos, double y_pos, bool clicking) override;
    void update_dimensions(int win_width, int win_height) override;
};
#endif
