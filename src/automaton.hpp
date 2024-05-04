#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include "shaders.hpp"
#include "constants.hpp"
#include "util/glm_colors.hpp"
#include "util/pxls.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace glm_colors;

class Automaton {
  private:
    int square_size;
    int cell_size;

    int win_height;
    int win_width;
    int hei_margin;
    int wid_margin;

    std::vector<glm::vec2> quad_vertices;

    void update_grid();
    void set_shaders();
    void set_square_vertices();

    virtual int apply_rules(int offset) = 0;

    float origin_x;
    float origin_y;

    unsigned int quad_VAO;
    unsigned int instance_VBOs[2];

  public:
    Automaton();
    Automaton(std::string path_str, int win_width, int win_height,
              int square_size);
    virtual ~Automaton(){};
    virtual std::string get_type() = 0;

    virtual void update() = 0;
    void update_dimensions(int win_width, int win_height);
    void update_square_size(int square_size);
    void clear(bool all);
    void draw();
    void set_value(double x_pos, double y_pos, int val, int radius);
    void toggle_plague();
    void change_cursor_shape();
    void save();
    void load();

    int get_cell_count();
    int get_square_size();
    int get_cell_size();

    void fill_random();

  protected:
    std::vector<glm::vec3> colors;

    int cell_count;
    int rows;
    int cols;

    bool plague;
    bool circular_cursor;

    std::vector<int> cells;
    std::vector<int> update_cells;

    void update_states();
    void set_cell_colors();

    Shader shader_program;
};
#endif
