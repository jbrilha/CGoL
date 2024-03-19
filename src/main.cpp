#include <mach-o/dyld.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libproc.h>
#include <string>
#include <unistd.h>
#include "gol.hpp"
#include "shaders.hpp"

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static void error_callback(int error, const char *description);
static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height);
static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
static void display_FPS(GLFWwindow *window, double current_time);
static void process_mouse_input(GLFWwindow *window, Gol *gol);

// float pixels_to_float(int pixels);

std::string GAME_NAME = "C GoL";
std::string VERSION = "v0.9";
std::string GAME_VERSION_NAME = VERSION + " | " + GAME_NAME;

// fps
int nb_frames = 0;
int last_time = 0;

// viewport settings
int win_width = 1200;
int win_height = 800;

bool seeding = true;
bool ready = false;
bool pressing = false;
bool update_size = false;

float delta_time = 0.f;
float last_frame = 0.f;

int counter = 0;
int delay = 10;

std::string get_executable_path(){
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
        printf("executable path is %s\n", path);
    else
        printf("buffer too small; need size %u\n", size);

    std::string str(path);
    std::string path_str = path;

    std::string clean_path(path_str.c_str(), path_str.c_str() + path_str.rfind("/"));
    std::string cleanest_path(clean_path.c_str(), clean_path.c_str() + clean_path.rfind("/"));

    return cleanest_path;
}

int main(int argc, char *argv[]) {

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window =
        glfwCreateWindow(win_width, win_height, GAME_VERSION_NAME.c_str(), NULL, NULL);

    if (!window) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // set callback functions
    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwMakeContextCurrent(window);
    // gladLoadGL();
    // limit frame_rate to display (kinda like V-Sync?)
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string path_str = get_executable_path();

    Shader shader_program((path_str + "/shaders/shader.vert").c_str(), (path_str + "/shaders/shader.frag").c_str());
    Gol gol(shader_program, win_width, win_height);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        counter++;
        process_mouse_input(window, &gol);

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        double current_time = glfwGetTime();
        nb_frames++;
        if (current_time - last_time >= 1.0)
            display_FPS(window, current_time);

        glClearColor(0.4f, 0.4f, 0.4f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 
        // std::cout << "\r"
        //     << "The current path " << p << " decomposes into:\n"
        //     << "root-path " << p.root_path() << '\n'
        //     << "relative path " << p.relative_path() << std::flush;

        // if(update_size) {
        //     gol.update_dimensions(win_width, win_height);
        //     update_size = false;
        //     std::cout << "updated dimensions" << std::endl;
        // }

        if (counter > delay) {
            if (!seeding && ready) {
                gol.update();
            }
            counter = 0;
        }
        gol.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader_program.program_ID);
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

static void process_mouse_input(GLFWwindow *window, Gol *gol) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    int m1_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (m1_state == GLFW_PRESS) {
        gol->set_value(x_pos, y_pos, 1);
        seeding = true;
    }
    int m2_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (m2_state == GLFW_PRESS) {
        gol->set_value(x_pos, y_pos, 0);
        seeding = true;
    }
    if (m2_state == GLFW_RELEASE && m1_state == GLFW_RELEASE) {
        seeding = false;
    }
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        ready = !ready;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    win_width = width;
    win_height = height;

    update_size = true;
}

static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void display_FPS(GLFWwindow *window, double current_time) {
    std::string NAME_FPS =
        GAME_VERSION_NAME + " - " + std::to_string(nb_frames) + " FPS | tickrate: " + std::to_string(delay) + ((ready && !seeding)? " | simulation running!" : " | seeding!");

    if(nb_frames < 60) {
        delay = std::min(delay, nb_frames / 4);
    }

    nb_frames = 0;
    last_time = current_time;

    glfwSetWindowTitle(window, NAME_FPS.c_str());
}

// float pixels_to_float(int pixels) { return 2.f * float(pixels) / height; }
