#ifndef RULE0_HPP
#define RULE0_HPP

#include "automaton.hpp"

using namespace glm_colors;

class Rule0 : public Automaton {
  public:
    Rule0(std::string path_str, int win_width, int win_height, int square_size);
    ~Rule0();
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;

};
#endif
