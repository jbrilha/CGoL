#ifndef BRAIN_HPP
#define BRAIN_HPP

#include "automaton.hpp"

class Brain : public Automaton {
  public:
    Brain(std::string path_str, int win_width, int win_height, int square_size);
    ~Brain() {};
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;
    void set_cell_colors() override;

};
#endif
