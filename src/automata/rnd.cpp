#include "rnd.hpp"

RND::RND(std::string path_str, GLFWwindow *window, int square_size)
    : Automaton(path_str, window, square_size, red) {

    colors.push_back(green);
    colors.push_back(black);
    colors.push_back(blue);
    set_cell_colors();
};

RND::~RND() { glDeleteProgram(shader_program.program_ID); }

void RND::update() {
    const int COUNT = 8;
    for (int offset = 0; offset < cell_count; offset++) {
        // for (int offset = cell_count - 1; offset >= 0; offset--) {
        int col = offset % cols;
        if (cells[offset]
            && offset >= 0
            && offset < cell_count
            && col < (cols - 1)
            && col > 0
        ) {
            int bot = offset + cols;
            int top = offset - cols;
            int lft = offset - 1;
            int rgt = offset + 1;

            const std::array<int, COUNT> nghbr_at = {
                top - 1, top, top + 1, lft, rgt, bot - 1, bot, bot + 1};

            // if (rand() % 100 > 66) {
                int rdx = rand() % nghbr_at.size();
                int idx = nghbr_at[rdx];
                if (idx < 0 || idx > cell_count) continue;
                update_cells[idx] = cells[offset];
            // }
        }
    }
    cells = update_cells;

    update_states();
}

std::string RND::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "nan: " + clean_name;
}
