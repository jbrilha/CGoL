#ifndef SAND_HPP
#define SAND_HPP

#include "automaton.hpp"

class Sand : public Automaton {
  public:
    Sand(std::string path_str, GLFWwindow *window, int square_size);
    ~Sand();
    void update() override;
    void update_chunk(int thread_idx, size_t thread_count) override;
    void update_cell_states() override;
    std::string get_type() override;

  private:
    bool look_ahead(int offset, int state);
};
#endif
