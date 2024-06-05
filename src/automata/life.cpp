#include "life.hpp"

Life::Life(std::string path_str, GLFWwindow *window, int square_size)
    : Automaton(path_str, window, square_size, green) {

    set_cell_colors();
};

Life::~Life() { glDeleteProgram(shader_program.program_ID); }

void Life::update() {
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

        if (state && (neighbors < 2 || neighbors > 3)) {
            if (!plague)
                update_cells[offset] = 0;
            else
                cells[offset] = 0;
        } else if (!state && neighbors == 3) {
            if (!plague)
                update_cells[offset] = 1;
            else
                cells[offset] = 1;
        } else {
            if (!plague)
                update_cells[offset] = state;
            else
                cells[offset] = state;
        }
    }
    if (!plague)
        cells = update_cells;

    update_states();
}

std::string Life::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "5: " + clean_name + (plague ? " (plague)" : "");
}
