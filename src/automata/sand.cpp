#include "sand.hpp"

Sand::Sand(std::string path_str, GLFWwindow *window, int square_size)
    : Automaton(path_str, window, square_size, sand_yellow) {

    // colors.push_back(brown);
    colors.push_back(sand_yellow);
    colors.push_back(river_blue);
    colors.push_back(purple);
    // colors.push_back(river_blue);
    colors.push_back(white);
    set_cell_colors();
};

Sand::~Sand() { glDeleteProgram(shader_program.program_ID); }

void Sand::update() {
    for (int offset = cell_count - 1; offset >= 0; offset--) {
        int state = cells[offset];
        if (state && state != SOLID) {
            state = state < 3 ? SAND : WATER;
            int B = offset + cols; // (B)ot

            if (B >= cell_count) {
                cells[offset] = state;
                continue;
            }

            int L = offset - 1; // (L)eft
            int R = offset + 1; // (R)ight

            int BL = B - 1; // (B)ot (L)eft
            int BR = B + 1; // (B)ot (R)ight

            bool go_b_left = false, go_b_right = false;

            if (state == WATER)
                goto water;

            if (!cells[B]) {
                update_cells[offset] = 0;
                update_cells[B] = FALLING_SAND;
                continue;
            } else {
                if (!cells[L] && (!cells[BL] || cells[BL] == FALLING_SAND) &&
                    !update_cells[BL] && look_ahead(L - 1, SAND) &&
                    (BL + 1) % cols
                    // && L > 0
                ) {
                    go_b_left = true;
                }
                if (!cells[R] && (!cells[BR] || cells[BR] == FALLING_SAND) &&
                    !update_cells[BR] && BR % cols
                    // && BR < cell_count
                ) {
                    go_b_right = true;
                }

                if (!(go_b_left || go_b_right)) {
                    update_cells[offset] = SAND;
                    continue;
                }

                int idx = 0;
                if (go_b_left && go_b_right) {
                    idx = (double)rand() / RAND_MAX < 0.5 ? BL : BR;
                } else {
                    idx = go_b_left ? BL : BR;
                }

                update_cells[idx] = SAND;
                update_cells[offset] = 0;
                continue;
            }

        water:
            bool go_left = false, go_right = false;
            // if (!(cells[B] % 2)) { // shower kinda
            // if (cells[B] && !(cells[B] % 2)) {
            if (!cells[B] && !update_cells[B]) {
                update_cells[offset] = 0;
                update_cells[B] = FALLING_WATER;
                continue;
            } else {
                if (!cells[L] &&
                    !update_cells[L]
                    // && look_ahead(offset - 1, WATER)
                    && look_ahead(L - 1, WATER) && (L + 1) % cols) {
                    if ((!cells[BL] || cells[BL] == FALLING_WATER) &&
                        !update_cells[BL] && look_ahead(L - 1, WATER) &&
                        (BL + 1) % cols
                        // && L > 0
                    ) {
                        go_b_left = true;
                    } else {
                        go_left = true;
                    }
                }
                if (!cells[R] &&
                    !update_cells[R]
                    // && look_ahead(L - 1, WATER)
                    && R % cols && R < cell_count) {
                    if ((!cells[BR] || cells[BR] == FALLING_WATER) &&
                        !update_cells[BR] && BR % cols
                        // && BR < cell_count
                    ) {
                        go_b_right = true;
                    } else {
                        go_right = true;
                    }
                }

                if (!(go_b_left || go_b_right || go_left || go_right)) {
                    update_cells[offset] = WATER;
                    continue;
                }

                int idx = 0;
                if (go_b_left && go_b_right) {
                    idx = (double)rand() / RAND_MAX < 0.5 ? BL : BR;
                } else if (go_b_left || go_b_right) {
                    idx = go_b_left ? BL : BR;
                } else if (go_right && go_left) {
                    idx = (double)rand() / RAND_MAX < 0.5 ? L : R;
                } else {
                    idx = go_left ? L : R;
                }

                update_cells[idx] = WATER;
                update_cells[offset] = 0;
                continue;
            }
        }
    }

    cells = update_cells;

    update_states();
}

bool Sand::look_ahead(int look_ahead_offset, int state) {
    if (cells[look_ahead_offset] == state && (double)rand() / RAND_MAX < 0.5) {
        return false;
    }

    return true;
}

std::string Sand::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "0: " + clean_name;
}
