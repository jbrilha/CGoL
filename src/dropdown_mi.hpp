#ifndef DROPDOWN_MENU_ITEM_HPP
#define DROPDOWN_MENU_ITEM_HPP

#include "shaders.hpp"
#include "constants.hpp"
#include "src/menu_item.hpp"
#include "util/glm_colors.hpp"
#include "util/pxls.hpp"
#include "src/button_mi.hpp"

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
    Dropdown(std::string path_str, GLFWwindow *window, int dimensions, glm::vec3 position);
    Dropdown(std::string path_str, GLFWwindow *window, int dimensions, glm::vec3 position, glm::vec3 color);
    ~Dropdown();

    void draw() override;
    void click() override;
};
#endif
