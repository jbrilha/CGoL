#ifndef SPIRAL_HPP
#define SPIRAL_HPP

#include "automaton.hpp"

using namespace glm_colors;

class Spiral : public Automaton {
  public:
    Spiral(std::string path_str, GLFWwindow *window, int square_size);
    ~Spiral();
    void update() override;
    void update_chunk(int thread_idx, size_t thread_count) override;
    void update_cell_states() override;
    std::string get_type() override;

  private:
};
#endif
