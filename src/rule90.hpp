#ifndef RULE90_HPP
#define RULE90_HPP

#include "automaton.hpp"

class Rule90 : public Automaton {
  public:
    Rule90(std::string path_str, int win_width, int win_height, int square_size);
    ~Rule90() {};
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;
    void set_cell_colors() override;

};
#endif
