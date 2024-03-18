#ifndef GOL_HPP
#define GOL_HPP

#include "shaders.hpp"
// #include "rect.hpp"
#include "square.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

static const int SQUARE_SIZE = 4;
static const int GAP = 1;

class Gol {
  public:
    // Gol(Shader shader_program, int win_size);
    Gol(Shader shader_program, int win_width, int win_height);
    void update();
    void update_dimensions(int win_width, int win_height);

    void draw();
    void render_cell(bool alive, int row, int col, glm::vec3 color1, glm::vec3 color2);
    void set_value(double x_pos, double y_pos, int val);
    void toggle_value(double x_pos, double y_pos);

  private:
    bool inbounds(int row, int col);
    int apply_rules(int row, int col);

    const Shader shader_program;
    // const int win_size;
    float win_height;
    float win_width;
    Square *square;
    int cell_size;
    int rows;
    int cols;
    float origin_x;
    float origin_y;

    std::vector<std::vector<int>> cells;
    std::vector<std::vector<int>> update_cells;

    float pxls_to_float(int pixels, int total_pixels);
};
#endif
