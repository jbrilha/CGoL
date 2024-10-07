#ifndef LIVE_FREE_OR_DIE_HPP
#define LIVE_FREE_OR_DIE_HPP

#include "automaton.hpp"

class LFoD : public Automaton {
  public:
    LFoD(std::string path_str, GLFWwindow *window, int square_size);
    ~LFoD();
    void update() override;
    void update_chunk(int thread_idx, size_t thread_count) override;
    void update_cell_states() override;
    std::string get_type() override;

  private:
};
#endif
