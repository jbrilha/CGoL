#ifndef DROPDOWN_MENU_ITEM_HPP
#define DROPDOWN_MENU_ITEM_HPP

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

class Dropdown : public MenuItem {
  private:
    glm::vec3 center;
    glm::vec3 offset;
    std::vector<MenuItem*> items;

    void set_vertices() override;
    void set_model() override;
    void collapse();
    void expand();

  public:
    Dropdown();
    Dropdown(std::string path_str, int win_width, int win_height, int square_size, int radius);
    ~Dropdown();

    void draw() override;
    void click() override;
    bool collapsed;
};
#endif
