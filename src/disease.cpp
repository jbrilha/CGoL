#include "disease.hpp"
Disease::Disease(std::string path_str, int win_width, int win_height, int square_size)
        : Automaton(path_str, win_width, win_height, square_size) {

    set_cell_colors();
};

void Disease::update() {
    int state = 0;

    for (int offset = 0; offset < cell_count; offset++) {
        state = cells[offset];

        int neighbors = apply_rules(offset);

        if (state && (neighbors >= 3 && neighbors <= 6)) {
            update_cells[offset] = 1;
        } else if (!state && (neighbors == 3 || neighbors == 6)) {
            update_cells[offset] = 1;
        } else {
            update_cells[offset] = 0;
        }
    }
    cells = update_cells;

    update_states();
}

int Disease::apply_rules(int offset) {
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

void Disease::set_cell_colors() {
    shader_program.set_vec3("color0", grey);
    shader_program.set_vec3("color1", light_blue);
}

std::string Disease::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "3: " + clean_name;
}
