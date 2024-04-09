#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include "shaders.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// static const int SQUARE_SIZE = 9;
static const int GAP = 1;

class Automaton {
  private:
    int win_height;
    int win_width;

    int square_size;
    int cell_size;

    void update_grid();
    void prepare_shaders();
    virtual void set_cell_colors() = 0;

    virtual int apply_rules(int offset) = 0;
    float pxls_to_float(int pixels, int total_pixels);

    float origin_x;
    float origin_y;

    unsigned int quadVAO;
    unsigned int instanceVBOs[2];

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

    void set_value(double x_pos, double y_pos, int val);
    void toggle_plague();
    int get_cell_count();
    int get_squqare_size();

    void fill_random();

  protected:
    int cell_count;
    bool plague;
    std::vector<int> cells;
    std::vector<int> update_cells;
    void update_states();
    int rows;
    int cols;

    Shader shader_program;
    glm::vec3 red = glm::vec3(1.f, 0.f, 0.f);
    glm::vec3 green = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 grey = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 white = glm::vec3(1.f, 1.f, 1.f);
    glm::vec3 pink = glm::vec3(1.f, 0.5f, 0.5f);
    glm::vec3 purple = glm::vec3(0.5f, 0.f, 0.7f);
    glm::vec3 brown = glm::vec3(0.4f, 0.2f, 0.f);
    glm::vec3 yellow = glm::vec3(1.f, 0.9f, 0.6f);
    glm::vec3 light_blue = glm::vec3(0.2f, 0.8f, 1.f);

};
#endif
