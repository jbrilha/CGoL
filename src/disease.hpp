#ifndef DISEASE_HPP
#define DISEASE_HPP

#include "automaton.hpp"

class Disease : public Automaton {
  public:
    Disease(std::string path_str, int win_width, int win_height, int square_size);
    ~Disease() {};
    void update() override;
    std::string get_type() override;

  private:
    int apply_rules(int offset) override;
    void set_cell_colors() override;

};
#endif
