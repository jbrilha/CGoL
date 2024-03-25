#ifndef LIFE_HPP
#define LIFE_HPP

#include "automaton.hpp"

class Life : public Automaton {
  public:
    Life(std::string path_str, int win_width, int win_height);
    ~Life() {};
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;
    void set_cell_colors() override;

};
#endif
