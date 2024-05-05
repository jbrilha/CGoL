#include "sand.hpp"
#include "src/automaton.hpp"

Sand::Sand(std::string path_str, int win_width, int win_height, int square_size)
    : Automaton(path_str, win_width, win_height, square_size) {

    colors.push_back(sand_yellow);
    // colors.push_back(brown);
    colors.push_back(sand_yellow);
    colors.push_back(ocean_blue);
    // colors.push_back(purple);
    colors.push_back(ocean_blue);
    colors.push_back(white);
    set_cell_colors();
};

Sand::~Sand() { glDeleteProgram(shader_program.program_ID); }

void Sand::update() {
    // update_cells = cells;
    for (int offset = cell_count - 1; offset >= 0; offset--) {
        // for (int offset = 0; offset < cell_count; offset++) {
        int state = cells[offset];
        if(state && state != SOLID) {
            apply_rules(offset);

        }
        // int a = apply_rules(offset);
        // if(a) {
        //     std::cout << "oof" << a << std::endl;
        // }
    }

    cells = update_cells;

    update_states();
}

int Sand::apply_rules(int offset) {
    int STATE = cells[offset] < 3 ? SAND : WATER;

    int B = offset + cols; // (B)ot

    if (B >= cell_count) {
        update_cells[offset] = STATE;
        return 10;
    }

    int L = offset - 1; // (L)eft
    int R = offset + 1; // (R)ight

    int BL = B - 1; // (B)ot (L)eft
    int BR = B + 1; // (B)ot (R)ight
    
    bool go_b_left = false, go_b_right = false;
    bool swap = false;
    if(STATE == WATER) goto water;

    if (cells[B]) {
        if(cells[B] == WATER) {
            update_cells[offset] = WATER;
            update_cells[B] = SAND;
            return 0;
        }
        if (!cells[L]
            && (!cells[BL]
            || cells[BL] == FALLING_SAND
        )
            && update_cells[BL] != SAND
            && (BL + 1) % cols
            && BL < cell_count
        ) {
            // if(cells[BL] == WATER) swap = true;
            go_b_left = true;
        }

        if (!cells[R]
            && (!cells[BR]
            || cells[BR] == FALLING_SAND
        )
            && update_cells[BR] != SAND
            && BR % cols
            && BR < cell_count
        ) {
            // if(cells[BR] == WATER) swap = true;
            go_b_right = true;
        }

        if (!(go_b_left || go_b_right)
        ) {
            update_cells[offset] = SAND;
            return 1;
        }

        int idx = 0;
        if (go_b_right && go_b_left) {
            idx = (double)rand() / RAND_MAX < 0.5 ? BL : BR;
        } else {
            idx = go_b_left ? BL : BR;
        }

        update_cells[idx] = SAND;
        update_cells[offset] = 0;
        return 2;
    } else {
        update_cells[offset] = 0;
        update_cells[B] = FALLING_SAND;
        return 3;
    }

water:
    bool go_left = false, go_right = false;
    // if (!(cells[B] % 2)) { // shower kinda
    // if (cells[B] && !(cells[B] % 2)) {
    if (cells[B]) {
        if (!cells[L]
            && (!cells[BL]
            || cells[BL] == FALLING_WATER
        )
            && update_cells[BL] != WATER
            && (BL + 1) % cols
            && BL > 0
        ) {
            go_b_left = true;
        }

        if (!cells[R]
            && (!cells[BR]
            || cells[BR] == FALLING_WATER
        )
            && update_cells[BR] != WATER
            && BR % cols
            && BR < cell_count
        ) {
            go_b_right = true;
        }

        if (!go_b_left
            && !cells[L]
            && update_cells[L] != WATER
            && (L + 1) % cols
            && L > 0
        ) {
            go_left = true;
        }
        if (!go_b_right
            && !cells[R]
            && update_cells[R] != WATER
            && R % cols
            && R < cell_count
        ) {
            go_right = true;
        }

        if (!(go_b_left || go_b_right
            || go_left || go_right
        )) {
            update_cells[offset] = WATER;
            return 1;
        }

        int idx = 0;
        if (go_b_right && go_b_left) {
            idx = (double)rand() / RAND_MAX < 0.5 ? BL : BR;
        } else if (go_b_right || go_b_left) {
            idx = go_b_left ? BL : BR;
        } else if (go_right && go_left) {
            idx = (double)rand() / RAND_MAX < 0.5 ? L : R;
        } else {
            idx = go_left ? L : R;
        }

        update_cells[idx] = WATER;
        update_cells[offset] = 0;
        return 2;
    } else {
        if((update_cells[B] % 2)) {
            update_cells[offset] = WATER;
            return 0;
        }

        update_cells[offset] = 0;
        update_cells[B] = FALLING_WATER;
        return 3;
    }

}

std::string Sand::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "9: " + clean_name;
}
