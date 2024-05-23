#ifndef RND_HPP
#define RND_HPP

#include "automaton.hpp"

using namespace glm_colors;

class RND : public Automaton {
  public:
    RND(std::string path_str, GLFWwindow *window, int square_size);
    ~RND();
    void update() override;
    std::string get_type() override;

  private:
};
#endif
