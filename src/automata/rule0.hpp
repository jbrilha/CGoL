#ifndef RULE0_HPP
#define RULE0_HPP

#include "automaton.hpp"

using namespace glm_colors;

class Rule0 : public Automaton {
  public:
    Rule0(std::string path_str, GLFWwindow *window, int square_size);
    ~Rule0();
    void update() override;
    std::string get_type() override;

  private:
};
#endif
