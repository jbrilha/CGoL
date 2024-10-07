#include "rule90.hpp"

Rule90::Rule90(std::string path_str, GLFWwindow *window, int square_size)
    : Automaton(path_str, window, square_size, red) {

    set_cell_colors();
};

Rule90::~Rule90() { glDeleteProgram(shader_program.program_ID); }

void Rule90::update() {
    for (int offset = 0; offset < cell_count; offset++) {
        update_cells[offset] = cells[offset - 1] != cells[offset + 1];
    }
    cells = update_cells;

    update_states();
}

void Rule90::update_chunk(int thread_idx, size_t thread_count) {
    int chunk_size = cell_count / thread_count;
    int start = thread_idx * chunk_size;
    int end = start + chunk_size;

    for (int offset = start; offset < end; offset++) {
        update_cells[offset] = cells[offset - 1] != cells[offset + 1];
    }
}

void Rule90::update_cell_states() {
    cells = update_cells;
    update_states();
}


std::string Rule90::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "8: " + clean_name;
}
