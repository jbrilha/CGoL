#ifndef SIMULATION_HPP
#define SIMULATION_HPP

// #include <mach-o/dyld.h> // MacOS only!!
#include <unistd.h>

#include "automaton.hpp"
#include "cursor.hpp"
#include "menu.hpp"
#include "constants.hpp"

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Simulation {
  public:
    Simulation(char *argv0);
    ~Simulation();

    void set_automaton(Automaton *automaton);
    void run();
    void init();

  private:
    std::string get_path(char *arg);
    std::string path_str;

    bool clicking;

    int win_height;
    int win_width;

    int radius;

    Automaton *automaton;
    Cursor *cursor;
    Menu *menu;

    GLFWwindow *window;
    void init_GLFW();

    void set_callbacks();
    void error_callback(int error, const char *description);
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void key_callback(GLFWwindow *window, int key, int scancode, int action,
                      int mods);
    void drop_callback(GLFWwindow *window, int count, const char **paths);
    void mouse_pos_callback(GLFWwindow *window, double x_pos_in,
                            double y_pos_in);
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void update_title_bar();
    void process_input();

    std::string GAME_NAME = "C GoL";
    std::string VERSION = "v1.1";
    std::string GAME_VERSION_NAME = VERSION + " | " + GAME_NAME;

    int nb_frames;
    int last_time;
    std::string FPS;

    int cell_count;

    bool seeding;
    bool ready;
    bool update_size;
    bool plague;
    bool step;
    bool toggle_val;

    float delta_time;
    float last_frame;

    int counter;
    int tickrate;
};
#endif
