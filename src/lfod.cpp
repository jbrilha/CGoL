#include "lfod.hpp"

LFoD::LFoD(std::string path_str, GLFWwindow *window, int square_size)
    : Automaton(path_str, window, square_size, red) {

    set_cell_colors();
};

LFoD::~LFoD() { glDeleteProgram(shader_program.program_ID); }

void LFoD::update() {
    int state = 0;

    for (int offset = 0; offset < cell_count; offset++) {
        state = cells[offset];

        int neighbors = apply_rules(offset);

        if (state && !neighbors) {
            update_cells[offset] = 1;
        } else if (!state && neighbors == 2) {
            update_cells[offset] = 1;
        } else {
            update_cells[offset] = 0;
        }
    }
    cells = update_cells;

    update_states();
}

int LFoD::apply_rules(int offset) {
    int neighbors = 0;
    int row = offset / cols;
    int col = offset % cols;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0)
                continue;

            int neigh_row = (row + i + rows) % rows;
            int neigh_col = (col + j + cols) % cols;
            int offset = neigh_row * cols + neigh_col;

            if (offset >= 0 && offset < cell_count && cells[offset]) {
                neighbors++;
            }
        }
    }

    return neighbors;
}

std::string LFoD::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "4: " + clean_name;
}
