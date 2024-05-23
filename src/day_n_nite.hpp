#ifndef DAY_N_NITE_HPP
#define DAY_N_NITE_HPP

#include "automaton.hpp"

class DayNNite : public Automaton {
  public:
    DayNNite(std::string path_str, GLFWwindow *window, int square_size);
    ~DayNNite();
    void update() override;
    std::string get_type() override;

  private:
};
#endif
