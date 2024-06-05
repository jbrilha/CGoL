#include "rule0.hpp"

Rule0::Rule0(std::string path_str, GLFWwindow *window, int square_size)
    : Automaton(path_str, window, square_size, blue) {

    set_cell_colors();
};

Rule0::~Rule0() { glDeleteProgram(shader_program.program_ID); }

void Rule0::update() {
    for (int offset = 0; offset < cell_count; offset++) {
        update_cells[offset] = cells[offset - cols] != cells[offset + cols];
    }
    cells = update_cells;

    update_states();
}

std::string Rule0::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "6: " + clean_name;
}
