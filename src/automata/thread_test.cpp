#include "thread_test.hpp"
#include <thread>

ThreadTest::ThreadTest(std::string path_str, GLFWwindow *window,
                       int square_size)
    : Automaton(path_str, window, square_size, red) {

    colors.push_back(blue);
    set_cell_colors();
};

ThreadTest::~ThreadTest() { glDeleteProgram(shader_program.program_ID); }

void ThreadTest::update() {
    // std::thread th1(&ThreadTest::update1, this);
    // std::thread th2(&ThreadTest::update2, this);
    auto lamb = [this](int thread_ID) {
        if (thread_ID == 2) {
            for (int offset = cell_count / 2; offset < cell_count; offset++) {
                int val = cells[offset - cols] != cells[offset + cols];

                update_cells[offset] = val == 1 ? 2 : 0;
            }
        } else if (thread_ID == 1) {
            for (int offset = 0; offset < cell_count / 2; offset++) {
                update_cells[offset] = cells[offset - 1] != cells[offset + 1];
            }
        }
    };

    std::thread th1(lamb, 1);
    std::thread th2(lamb, 2);

    th1.join();
    th2.join();

    std::cout << "threads joined" << std::endl;

    cells = update_cells;

    update_states();
}

void ThreadTest::update1() {
    std::cout << "th1" << std::endl;
    for (int offset = 0; offset < cell_count / 2; offset++) {
        update_cells[offset] = cells[offset - 1] != cells[offset + 1];
    }
}

void ThreadTest::update2() {
    std::cout << "th2" << std::endl;
    for (int offset = cell_count / 2; offset < cell_count; offset++) {
        int val = cells[offset - cols] != cells[offset + cols];

        update_cells[offset] = val == 1 ? 2 : 0;
    }
}

std::string ThreadTest::get_type() {
    std::string type_name = typeid(*this).name();
    std::string clean_name(type_name.begin() + 1, type_name.end());

    return "7: " + clean_name;
}
