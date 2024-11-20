#include "thread_test.hpp"
#include <thread>

ThreadTest::ThreadTest(std::string path_str, GLFWwindow *window,
                       int square_size)
    : Automaton(path_str, window, square_size, red) {

    colors.push_back(blue);
    colors.push_back(purple);
    colors.push_back(pink);
    colors.push_back(yellow);
    colors.push_back(green);
    colors.push_back(brown);
    colors.push_back(orange);
    set_cell_colors();
};

ThreadTest::~ThreadTest() { glDeleteProgram(shader_program.program_ID); }

void ThreadTest::update() {
    // auto lamb = [this](int thread_ID) {
    //     if (thread_ID == 2) {
    //         for (int offset = cell_count / 2; offset < cell_count; offset++) {
    //             int val = cells[offset - cols] != cells[offset + cols];
    //
    //             update_cells[offset] = val == 1 ? 2 : 0;
    //         }
    //     } else if (thread_ID == 1) {
    //         for (int offset = 0; offset < cell_count / 2; offset++) {
    //             update_cells[offset] = cells[offset - 1] != cells[offset + 1];
    //         }
    //     }
    // };
    //
    // std::thread th1(lamb, 1);
    // std::thread th2(lamb, 2);
    //
    // th1.join();
    // th2.join();

    for (int offset = 0; offset < cell_count; offset++) {
        int val = (cells[offset - (cols)] != cells[offset + (cols)]) !=
           (cells[offset - 1] != cells[offset + 1]);
        update_cells[offset] = val;
    }

    cells = update_cells;

    update_states();
}

void ThreadTest::update_chunk(int thread_idx, size_t thread_count) {
    int chunk_size = cell_count / thread_count;
    int start = thread_idx * chunk_size;
    int end = start + chunk_size;

    for (int offset = start; offset < end; offset++) {
        int val = (cells[offset - (cols)] != cells[offset + (cols)]) !=
           (cells[offset - 1] != cells[offset + 1]);
        update_cells[offset] = val == 0 ? 0 : val + thread_idx;
    }
}

void ThreadTest::update_cell_states() {
    cells = update_cells;

    update_states();
}

std::string ThreadTest::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "7: " + clean_name;
}
