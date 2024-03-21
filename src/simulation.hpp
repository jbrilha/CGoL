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

const int WIN_WIDTH = 1400;
const int WIN_HEIGHT = 800;

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
    void display_FPS(GLFWwindow *window, double current_time);
    void process_mouse_input();

    std::string GAME_NAME = "C GoL";
    std::string VERSION = "v0.9";
    std::string GAME_VERSION_NAME = VERSION + " | " + GAME_NAME;

    // fps
    int nb_frames = 0;
    int last_time = 0;

    // viewport settings
    int cell_count = 0;

    bool seeding;
    bool ready;
    bool pressing;
    bool update_size;
    bool plague;

    float delta_time = 0.f;
    float last_frame = 0.f;

    int counter = 0;
    int delay = 10;

};
#endif
