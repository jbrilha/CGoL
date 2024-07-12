#ifndef MENU_HPP
#define MENU_HPP

#include "shaders.hpp"
#include "utils/constants.hpp"
#include "utils/glm_colors.hpp"
#include "utils/pxls.hpp"
#include "menu_item.hpp"
#include "mi_dropdown.hpp"
#include "mi_radial.hpp"

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm_colors;

class Menu {
  private:
    std::vector<MenuItem*> items;
    int win_width;
    int win_height;

  public:
    Menu();
    // Menu(std::string path_str, int win_width, int win_height, int square_size, int radius);
    Menu(std::string path_str, GLFWwindow *window);
    ~Menu();

    void draw();
    void click();
    void update_position(double x_pos, double y_pos);
    void update_dimensions(int win_width, int win_height);
    int handle_cursor(double x_pos, double y_pos, bool clicking);
};
#endif
