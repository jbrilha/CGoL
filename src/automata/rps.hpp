#ifndef RPS_HPP
#define RPS_HPP

#include "automaton.hpp"

using namespace glm_colors;
using std::cout, std::endl;

class RPS : public Automaton {
  public:
    RPS(std::string path_str, GLFWwindow *window, int square_size);
    ~RPS();
    void update() override;
    void update_chunk(int thread_idx, size_t thread_count) override;
    void update_cell_states() override;
    std::string get_type() override;

  private:
};
#endif
