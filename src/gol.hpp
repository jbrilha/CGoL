#ifndef GOL_HPP
#define GOL_HPP

#include "shaders.hpp"
#include "square.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

static const int SQUARE_SIZE = 9;
static const int GAP = 1;

class Gol {
  public:
    Gol(Shader shader_program, int win_size);
    void draw();
    bool seeding;
    void set_value(double x_pos, double y_pos, int val);

  private:
    void init();
    bool inbounds(int row, int col);
    void apply_rules(int row, int col);
    int counter = 0;
    int delay = 20;

    Shader shader_program;
    const int win_size;
    Square *square;
    int cell_size;
    int rows;
    int cols;
    float origin_x;
    float origin_y;
    std::vector<std::vector<int>> cells;

    float pxls_to_float(int pixels);
};
#endif
