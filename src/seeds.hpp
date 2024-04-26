#ifndef SEEDS_HPP
#define SEEDS_HPP

#include "automaton.hpp"

class Seeds : public Automaton {
  public:
    Seeds(std::string path_str, int win_width, int win_height, int square_size);
    ~Seeds();
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;

};
#endif
