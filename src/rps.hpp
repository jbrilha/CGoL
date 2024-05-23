#ifndef RPS_HPP
#define RPS_HPP

#include "automaton.hpp"

using namespace glm_colors;

class RPS : public Automaton {
  public:
    RPS(std::string path_str, GLFWwindow *window, int square_size);
    ~RPS();
    void update() override;
    std::string get_type() override;

  private:
};
#endif
