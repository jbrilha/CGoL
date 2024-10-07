#ifndef RULE90_HPP
#define RULE90_HPP

#include "automaton.hpp"

using namespace glm_colors;

class Rule90 : public Automaton {
  public:
    Rule90(std::string path_str, GLFWwindow *window, int square_size);
    ~Rule90();
    void update() override;
    void update_chunk(int thread_idx, size_t thread_count) override;
    void update_cell_states() override;
    std::string get_type() override;

  private:
};
#endif
