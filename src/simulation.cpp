#include "simulation.hpp"

Simulation::Simulation() : path_str(get_executable_path()), win_width(WIN_WIDTH), win_height(WIN_HEIGHT)
{
    if (path_str.empty()) {
        std::cerr << "Error: Failed to retrieve executable path" << std::endl;
        exit(EXIT_FAILURE);
    }

    init_GLFW();
}
Simulation::~Simulation() {
    delete life;
    life = nullptr;
}

void Simulation::init_life() {
    life = new Gol(path_str, win_width, win_height);
}

bool Simulation::run(){
    while (!glfwWindowShouldClose(window)) {
        counter++;
        process_mouse_input();

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        double current_time = glfwGetTime();
        nb_frames++;
        if (current_time - last_time >= 1.0)
            display_FPS(window, current_time);

        glClearColor(0.4f, 0.4f, 0.4f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        life->plague = plague;

        if (counter > delay) {
            if (!seeding && ready) {
                life->update();
            }
            counter = 0;
        }
        life->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glDeleteProgram(shader_program.program_ID);
    glfwDestroyWindow(window);

    glfwTerminate();
    return false;
}

std::string Simulation::get_executable_path(){
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

 void Simulation::process_mouse_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    int m1_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (m1_state == GLFW_PRESS) {
        life->set_value(x_pos, y_pos, 1);
        seeding = true;
    }
    int m2_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (m2_state == GLFW_PRESS) {
        life->set_value(x_pos, y_pos, 0);
        seeding = true;
    }
    if (m2_state == GLFW_RELEASE && m1_state == GLFW_RELEASE) {
        seeding = false;
    }
}

void Simulation::set_callbacks() {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Simulation* instance = static_cast<Simulation*>(glfwGetWindowUserPointer(window));
        if (instance)
            instance->key_callback(window, key, scancode, action, mods);
    });
    glfwSetErrorCallback([](int error, const char* description) {
        GLFWwindow* curr_window = glfwGetCurrentContext();
        if (curr_window != nullptr) {
            Simulation* instance = static_cast<Simulation*>(glfwGetWindowUserPointer(curr_window));
            if (instance)
                instance->error_callback(error, description);
        } else {
            fprintf(stderr, "Error (No window available): %s\n", description);
        }
    });
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        Simulation* instance = static_cast<Simulation*>(glfwGetWindowUserPointer(window));
        if (instance)
            instance->framebuffer_size_callback(window, width, height);
    });
}

void Simulation::key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        ready = !ready;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        plague = !plague;
    }
}

void Simulation::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    win_width = width;
    win_height = height;

    // update_size = true;
}

void Simulation::error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void Simulation::display_FPS(GLFWwindow *window, double current_time) {
    std::string NAME_FPS =
        GAME_VERSION_NAME + " - " + std::to_string(nb_frames) + " FPS | tickrate: " + std::to_string(delay)
                + " | " + std::to_string(cell_count) + " cells"
                + " | " + ((ready && !seeding)? (plague ?  "plague!" : "life!") : "seeding!");

    delay = std::max(delay, nb_frames / 10);

    nb_frames = 0;
    last_time = current_time;

    glfwSetWindowTitle(window, NAME_FPS.c_str());
}

void Simulation::init_GLFW() {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window =
        glfwCreateWindow(win_width, win_height, GAME_VERSION_NAME.c_str(), NULL, NULL);
        std::cout << window << std::endl;

    if (!window) {
        std::cout << window << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    set_callbacks();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    // limit frame_rate to display (kinda like V-Sync?)
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
}
