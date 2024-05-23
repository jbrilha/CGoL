#include "seeds.hpp"

Seeds::Seeds(std::string path_str, GLFWwindow *window, int square_size)
    : Automaton(path_str, window, square_size, orange) {

    set_cell_colors();
};

Seeds::~Seeds() { glDeleteProgram(shader_program.program_ID); }

void Seeds::update() {
    int state = 0;

    for (int offset = 0; offset < cell_count; offset++) {
        state = cells[offset];

        int neighbors = 0;
        int bot = offset + cols;
        int top = offset - cols;
        int lft = offset - 1;
        int rgt = offset + 1;
        std::array<int, 8> nghbr_at = {top - 1, top, top + 1, lft,
                                           rgt,     bot - 1, bot,     bot + 1};
        for (const int at : nghbr_at) {
            if (at >= 0 && at < cell_count && cells[at])
                neighbors++;
        }

        if (!state && neighbors == 2) {
            update_cells[offset] = 1;
            continue;
        }

        update_cells[offset] = 0;
    }
    cells = update_cells;

    update_states();
}

std::string Seeds::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "9: " + clean_name;
}
