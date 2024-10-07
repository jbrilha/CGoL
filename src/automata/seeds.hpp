#ifndef SEEDS_HPP
#define SEEDS_HPP

#include "automaton.hpp"

class Seeds : public Automaton {
  public:
    Seeds(std::string path_str, GLFWwindow *window, int square_size);
    ~Seeds();
    void update() override;
    void update_chunk(int thread_idx, size_t thread_count) override;
    void update_cell_states() override;
    std::string get_type() override;

  private:
};
#endif
