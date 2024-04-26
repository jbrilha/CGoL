#ifndef RULE180_HPP
#define RULE180_HPP

#include "automaton.hpp"

using namespace glm_colors;

class Rule180 : public Automaton {
  public:
    Rule180(std::string path_str, int win_width, int win_height, int square_size);
    ~Rule180();
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;

};
#endif
