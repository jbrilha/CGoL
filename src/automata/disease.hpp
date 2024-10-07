#ifndef DISEASE_HPP
#define DISEASE_HPP

#include "automaton.hpp"

class Disease : public Automaton {
  public:
    // Disease(std::string path_str, int win_width, int win_height,
    //         int square_size);
    Disease(std::string path_str, GLFWwindow *window, int square_size);
    ~Disease();
    void update() override;
    void update_chunk(int thread_idx, size_t thread_count) override;
    void update_cell_states() override;
    std::string get_type() override;

  private:
};
#endif
