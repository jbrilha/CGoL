#include "sand.hpp"
#include "util/glm_colors.hpp"
#include <cstdlib>

Sand::Sand(std::string path_str, int win_width, int win_height, int square_size)
    : Automaton(path_str, win_width, win_height, square_size) {

    // colors.push_back(black);
    colors.push_back(yellow);
    colors.push_back(white);
    colors.push_back(brown);
    set_cell_colors();
};

Sand::~Sand() { glDeleteProgram(shader_program.program_ID); }

void Sand::update() {
    for (int offset = cell_count - 1; offset >= 0; offset--) {
        apply_rules(offset);
    }
    cells = update_cells;

    update_states();
}

int Sand::apply_rules(int offset) {
    int above_offset = offset - cols;

    int under_offset = offset + cols;
    int left_under_offset = offset + cols - 1;
    int right_under_offset = offset + cols + 1;

    int left_offset = offset - 1;
    int right_offset = offset + 1;

    int state = cells[offset];

    if (state == 0 || state == 2) {
        return 0;
    }

    if (under_offset >= cell_count) {
        update_cells[offset] = 1;
        return 0;
    }
    if (cells[under_offset] == 1 || cells[under_offset] == 2) {
        if((double) rand() / RAND_MAX >= 0.5 ) goto check_right;

        if (!cells[left_offset] && !cells[left_under_offset]) {
            update_cells[offset] = 0;
            update_cells[left_under_offset] = 1;
        } else if (!cells[right_offset] && !cells[right_under_offset]) {
            update_cells[offset] = 0;
            update_cells[right_under_offset] = 1;
        } else {
            update_cells[offset] = 1;
        }
        return 0;
    check_right:
        if (!cells[right_offset] && !cells[right_under_offset]) {
            update_cells[offset] = 0;
            update_cells[right_under_offset] = 1;
        } else if (!cells[left_offset] && !cells[left_under_offset]) {
            update_cells[offset] = 0;
            update_cells[left_under_offset] = 1;
        } else {
            update_cells[offset] = 1;
        }
        return 0;
    } else {
        if (cells[above_offset] == 3 && cells[under_offset] == 0) {
            update_cells[offset] = 3;
            update_cells[above_offset] = 0;
            update_cells[under_offset] = cells[offset];
            return 0;
        } else if (cells[above_offset] == 0 || cells[above_offset] == 2) {
            update_cells[offset] = 0;
            update_cells[under_offset] = cells[offset];
            return 0;
        } else {
            update_cells[offset] = 3;
        }
    }

    return -1;
}
// int Sand::apply_rules(int offset) {
//     int above_offset = offset - cols;
//
//     int under_offset = offset + cols;
//     int left_above_offset = offset + cols - 1; // these are actually still under
//     int right_above_offset = offset + cols + 1;
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
