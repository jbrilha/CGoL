#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include "shaders.hpp"
#include "util/glm_colors.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

static const int GAP = 1;

using namespace glm_colors;

class Automaton {
  private:
    int square_size;
    int cell_size;

    int win_height;
    int win_width;
    int hei_margin;
    int wid_margin;

    bool allow_oob; // out of bounds

    void update_grid();
    void prepare_shaders();
    virtual void set_cell_colors() = 0;

    virtual int apply_rules(int offset) = 0;
    float pxls_to_float(int pixels, int total_pixels);

    float origin_x;
    float origin_y;

    unsigned int quadVAO;
    unsigned int instance_VBOs[2];

  public:
    Automaton();
    Automaton(std::string path_str, int win_width, int win_height, int square_size);
    virtual ~Automaton(){};
    virtual std::string get_type() = 0;

    virtual void update() = 0;
    void update_dimensions(int win_width, int win_height);
    void update_cell_size(int val);

    void clear();
    void draw();

    void set_value(double x_pos, double y_pos, int val, int radius);
    void toggle_plague();
    int get_cell_count();
    int get_square_size();

    void fill_random();

  protected:
    int cell_count;
    int rows;
    int cols;

    bool plague;

    std::vector<int> cells;
    std::vector<int> update_cells;

    void update_states();

    Shader shader_program;
};
#endif
