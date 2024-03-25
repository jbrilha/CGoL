#include "seeds.hpp"
Seeds::Seeds(std::string path_str, int win_width, int win_height)
        : Automaton(path_str, win_width, win_height) {

    set_cell_colors();
};

void Seeds::update() {
    int state = 0;

    for (int offset = 0; offset < cell_count; offset++) {
        state = cells[offset];

        int neighbors = apply_rules(offset);

        if(!state && neighbors == 2) {
            update_cells[offset] = 1;
            continue;
        }

        update_cells[offset] = 0;

    }
    cells = update_cells;

    update_states();
}

int Seeds::apply_rules(int offset) {
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

void Seeds::set_cell_colors() {
    shader_program.set_vec3("color0", grey);
    shader_program.set_vec3("color1", red);
}

std::string Seeds::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return clean_name;
}
