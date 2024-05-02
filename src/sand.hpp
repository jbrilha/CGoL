#ifndef SAND_HPP
#define SAND_HPP

#include "automaton.hpp"

static const int SAND = 1;
static const int FALLING_SAND = 2;
static const int WATER = 3;
static const int FALLING_WATER = 4;
static const int SOLID = 5;

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
