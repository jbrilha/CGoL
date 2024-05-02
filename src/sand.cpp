#include "sand.hpp"

Sand::Sand(std::string path_str, int win_width, int win_height, int square_size)
    : Automaton(path_str, win_width, win_height, square_size) {

    colors.push_back(yellow);
    // colors.push_back(brown);
    colors.push_back(yellow);
    colors.push_back(blue);
    colors.push_back(blue);
    colors.push_back(white);
    set_cell_colors();
};

Sand::~Sand() { glDeleteProgram(shader_program.program_ID); }

void Sand::update() {
    // update_cells = cells;
    for (int offset = cell_count - 1; offset >= 0; offset--) {
    // for (int offset = 0; offset < cell_count; offset++) {
        apply_rules(offset);
    }

    cells = update_cells;

    update_states();
}

int Sand::apply_rules(int offset) {
    int B = offset + cols; // (B)ot

    int state = cells[offset];

    if (state == SOLID || !state) {
        return 0;
    }

    if (B >= cell_count) {
        // update_cells[offset] = 1;
        update_cells[offset] = state < 3 ? SAND : WATER;
        return 1;
    }

    int L = offset - 1; // (L)eft
    int R = offset + 1; // (R)ight

    int BL = B - 1; // (B)ot (L)eft
    int BR = B + 1; // (B)ot (R)ight
    
    if(state > 2) goto water;

    if(cells[B]) {
        bool go_left = false, go_right = false;

        if( !cells[L]
            && (!cells[BL] || (cells[BL]) == FALLING_SAND)
            && update_cells[BL] != SAND
            && (BL + 1) % cols && BL + cols < cell_count
        ) {
            go_left = true;
        }
        if( !cells[R]
            && (!cells[BR] || cells[BR] == FALLING_SAND)
            && update_cells[BR] != SAND
            && BR % cols && BR + cols < cell_count
        ) {
            go_right = true;
        }

        if(!go_left && !go_right){
            update_cells[offset] = SAND;
            return 1;
        }

        int idx = 0;
        if (go_right && go_left) {
            idx = (double)rand() / RAND_MAX >= 0.5 ? BL : BR;
        } else {
            idx = go_left ? BL : BR;
        }

        update_cells[idx] =  SAND;
        update_cells[offset] = 0;
        return 2;
    } else {
        update_cells[offset] = 0;
        update_cells[B] =  FALLING_SAND;
        return 3;
    }
water:
    if(cells[B] < 4 && cells[B]) {
        bool go_left = false, go_right = false;
        bool go_b_left = false, go_b_right = false;

        if( !cells[L] && (L + 1) % cols && L > 0) {
            if( (!cells[BL] /*|| (cells[BL]) == FALLING_WATER*/)
                // && update_cells[BL] != WATER
                // && (BL + 1) % cols && BL + cols < cell_count)
            ){
                go_b_left = true;
            } else {
                go_left = true;
            }
        }
        if( !cells[R] && R % cols && R < cell_count) {
            if( (!cells[BR] /* || (cells[BR]) == FALLING_WATER */)
                // && update_cells[BR] != WATER
                // && BR % cols && BR + cols < cell_count) 
        ){
                go_b_right = true;
            } else {
                go_right = true;
            }
        }

        if(!go_left && !go_right
            && !go_b_left && !go_b_right
        ){
            update_cells[offset] = WATER;
            return 1;
        }

        int idx = 0;
        if (go_b_right && go_b_left) {
            idx = (double)rand() / RAND_MAX >= 0.5 ? BL : BR;
        } else if (go_b_right || go_b_left) {
            idx = go_b_left ? BL : BR;
        } else if (go_right && go_left) {
            idx = (double)rand() / RAND_MAX >= 0.5 ? L : R;
        } else if (go_right || go_left) {
            idx = go_left ? L : R;
        }

        update_cells[idx] =  WATER;
        update_cells[offset] = 0;
        return 2;
    } else {
        update_cells[offset] = 0;
        update_cells[B] =  FALLING_WATER;
        return 3;
    }

}

// int Sand::apply_rules(int offset) {
//     int above_offset = offset - cols;
//
//     int under_offset = offset + cols;
//     int left_above_offset = offset + cols - 1; // these are actually still
//     under int right_above_offset = offset + cols + 1;
//
//     int left_offset = offset - 1;
//     int right_offset = offset + 1;
//
//     int state = cells[offset];
//
//     if (state == 0 || state == 2) {
//         return 0;
//     }
//
//     if (above_offset >= cell_count) {
//         update_cells[offset] = 1;
//         return 0;
//     }
//     if (cells[above_offset] == 1 || cells[above_offset] == 2) {
//         if((double) rand() / RAND_MAX >= 0.5 ) goto check_right;
//
//         if (!cells[left_offset] && !cells[left_above_offset]) {
//             update_cells[offset] = 0;
//             update_cells[left_above_offset] = 1;
//         } else if (!cells[right_offset] && !cells[right_above_offset]) {
//             update_cells[offset] = 0;
//             update_cells[right_above_offset] = 1;
//         } else {
//             update_cells[offset] = 1;
//         }
//         return 0;
//     check_right:
//         if (!cells[right_offset] && !cells[right_above_offset]) {
//             update_cells[offset] = 0;
//             update_cells[right_above_offset] = 1;
//         } else if (!cells[left_offset] && !cells[left_above_offset]) {
//             update_cells[offset] = 0;
//             update_cells[left_above_offset] = 1;
//         } else {
//             update_cells[offset] = 1;
//         }
//         return 0;
//     } else {
//         if (cells[under_offset] == 3 && cells[above_offset] == 0) {
//             update_cells[offset] = 3;
//             update_cells[under_offset] = 0;
//             update_cells[above_offset] = cells[offset];
//             return 0;
//         } else if (cells[under_offset] == 0 || cells[under_offset] == 2) {
//             update_cells[offset] = 0;
//             update_cells[above_offset] = cells[offset];
//             return 0;
//         } else {
//             update_cells[offset] = 3;
//         }
//     }
//
//     return -1;
// }

std::string Sand::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "9: " + clean_name;
}
