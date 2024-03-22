#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <libproc.h>
#include <mach-o/dyld.h>

#include "gol.hpp"

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 400;

class Simulation {
  public:
    Simulation();
    ~Simulation();

    bool run();

    void init_life();

  private:
    std::string get_executable_path();
    std::string path_str;

    int win_height;
    int win_width;
    Gol *life;

    GLFWwindow *window;
    void init_GLFW();

    void set_callbacks();
    void error_callback(int error, const char *description);
    void framebuffer_size_callback(GLFWwindow *window, int width,
                                          int height);
    void key_callback(GLFWwindow *window, int key, int scancode,
                             int action, int mods);
    void update_FPS(double current_time);
    void update_title_bar();
    void process_mouse_input();

    std::string GAME_NAME = "C GoL";
    std::string VERSION = "v0.9";
    std::string GAME_VERSION_NAME = VERSION + " | " + GAME_NAME;

    int nb_frames;
    int last_time;
    std::string FPS;

    int cell_count;

    bool seeding;
    bool ready;
    bool update_size;
    bool plague;

    float delta_time;
    float last_frame;

    int counter;
    int delay;
};
#endif
