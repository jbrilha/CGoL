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
    Gol(Shader shader_program, int win_width, int win_height);
    void update();
    void update_states();
    void update_dimensions(int win_width, int win_height);

    void draw();
    void render_cell(bool alive, int row, int col, glm::vec3 color1, glm::vec3 color2);
    void set_value(double x_pos, double y_pos, int val);
    void toggle_value(double x_pos, double y_pos);

    const int cell_size;

  private:
    bool inbounds(int row, int col);
    int apply_rules(int row, int col);
    void prepare_shaders();

    Shader shader_program;
    float win_height;
    float win_width;
    Square *square;
    int rows;
    int cols;
    float origin_x;
    float origin_y;
    unsigned int quadVAO;
    unsigned int instanceVBOs[2];

    std::vector<int> states_vec;

    std::vector<std::vector<int>> cells;
    std::vector<std::vector<int>> update_cells;

    float pxls_to_float(int pixels, int total_pixels);
};
#endif
