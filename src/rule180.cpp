#include "rule180.hpp"

Rule180::Rule180(std::string path_str, int win_width, int win_height, int square_size)
    : Automaton(path_str, win_width, win_height, square_size) {

    set_cell_colors();
};

Rule180::~Rule180() {
    glDeleteProgram(shader_program.program_ID);
}

void Rule180::update() {
    for (int offset = 0; offset < cell_count; offset++) {
        update_cells[offset] = apply_rules(offset);
    }
    cells = update_cells;

    update_states();
}

int Rule180::apply_rules(int offset) {
    // if(!((offset) % cols)) return 0;
    return (cells[offset - (cols)] != cells[offset + (cols)]) != (cells[offset - 1] != cells[offset + 1]);
}

void Rule180::set_cell_colors() {
    shader_program.set_vec3("color0", black);
    shader_program.set_vec3("color1", purple);
}

std::string Rule180::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "7: " + clean_name;
}
