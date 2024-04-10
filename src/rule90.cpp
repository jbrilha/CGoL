#include "rule90.hpp"

Rule90::Rule90(std::string path_str, int win_width, int win_height, int square_size)
    : Automaton(path_str, win_width, win_height, square_size) {

    set_cell_colors();
};

Rule90::~Rule90() {
    glDeleteProgram(shader_program.program_ID);
}

void Rule90::update() {
    for (int offset = 0; offset < cell_count; offset++) {
        update_cells[offset] = apply_rules(offset);
    }
    cells = update_cells;

    update_states();
}

int Rule90::apply_rules(int offset) {
    return cells[offset - 1] != cells[offset + 1];
}

void Rule90::set_cell_colors() {
    shader_program.set_vec3("color0", grey);
    shader_program.set_vec3("color1", red);
}

std::string Rule90::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "6: " + clean_name;
}
