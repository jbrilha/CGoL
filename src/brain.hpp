#ifndef BRAIN_HPP
#define BRAIN_HPP

#include "automaton.hpp"

class Brain : public Automaton {
  public:
    Brain(std::string path_str, GLFWwindow *window, int square_size);
    ~Brain();
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;
};
#endif
