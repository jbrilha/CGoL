#ifndef BRAIN_HPP
#define BRAIN_HPP

#include "automaton.hpp"

class Brain : public Automaton {
  public:
    Brain(std::string path_str, GLFWwindow *window, int square_size);
    ~Brain();
    void update() override;
    void update_chunk(int thread_idx, size_t thread_count) override;
    void update_cell_states() override;
    std::string get_type() override;

  private:
};
#endif
