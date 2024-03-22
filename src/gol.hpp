#ifndef GOL_HPP
#define GOL_HPP

#include "shaders.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

static const int SQUARE_SIZE = 4;
static const int GAP = 1;

class Gol {
  public:
    Gol();
    Gol(std::string path_str, int win_width, int win_height);

    void update();
    void update_dimensions(int win_width, int win_height);

    void clear();
    void draw();

    void set_value(double x_pos, double y_pos, int val);
    void toggle_plague();
    void fill_random();

    bool plague;

    float win_height;
    float win_width;

    int cell_size;
    int cell_count;

  private:
    void prepare_shaders();
    void update_states();
    void render_cell(bool alive, int row, int col, glm::vec3 color1, glm::vec3 color2);

    char apply_rules(int offset);
    float pxls_to_float(int pixels, int total_pixels);

    Shader shader_program;

    int rows;
    int cols;

    float origin_x;
    float origin_y;

    unsigned int quadVAO;
    unsigned int instanceVBOs[2];

    std::vector<int> cells;
    std::vector<int> update_cells;

};
#endif
