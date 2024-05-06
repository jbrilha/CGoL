#ifndef SAND_HPP
#define SAND_HPP

#include "automaton.hpp"

class Sand : public Automaton {
  public:
    Sand(std::string path_str, int win_width, int win_height, int square_size);
    ~Sand();
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;
};
#endif
