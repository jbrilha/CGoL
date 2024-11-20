#ifndef THREAD_TEST_HPP
#define THREAD_TEST_HPP

#include "automaton.hpp"

using namespace glm_colors;

class ThreadTest : public Automaton {
  public:
    ThreadTest(std::string path_str, GLFWwindow *window, int square_size);
    ~ThreadTest();
    void update() override;
    void update_chunk(int thread_idx, size_t thread_count) override;
    void update_cell_states() override;
    std::string get_type() override;

  private:
    void update1();
    void update2();
};
#endif
