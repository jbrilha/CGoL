#ifndef LIFE_HPP
#define LIFE_HPP

#include "automaton.hpp"

class Life : public Automaton {
  public:
    Life(std::string path_str, GLFWwindow *window, int square_size);
    ~Life();
    void update() override;
    void update_chunk(int thread_idx, size_t thread_count) override;
    void update_cell_states() override;
    std::string get_type() override;

  private:
};
#endif
