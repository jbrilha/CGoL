#ifndef LIVE_FREE_OR_DIE_HPP
#define LIVE_FREE_OR_DIE_HPP

#include "automaton.hpp"

class LFod : public Automaton {
  public:
    LFod(std::string path_str, int win_width, int win_height);
    ~LFod() {};
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;
    void set_cell_colors() override;

};
#endif
