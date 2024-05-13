#ifndef LIFE_HPP
#define LIFE_HPP

#include "automaton.hpp"

class Life : public Automaton {
  public:
    Life(std::string path_str, GLFWwindow *window, int square_size);
    ~Life();
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;
};
#endif
