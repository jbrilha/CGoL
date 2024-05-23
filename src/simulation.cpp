#include "simulation.hpp"
#include "brain.hpp"
#include "day_n_nite.hpp"
#include "disease.hpp"
#include "lfod.hpp"
#include "life.hpp"
#include "rule0.hpp"
#include "rps.hpp"
#include "rnd.hpp"
#include "rule180.hpp"
#include "rule90.hpp"
#include "sand.hpp"
#include "seeds.hpp"
#include "src/automaton.hpp"
#include "src/constants.hpp"
#include <cstring>

Simulation::Simulation(char *argv0)
    : path_str(get_path(argv0)), clicking(false), win_height(WIN_HEIGHT),
      win_width(WIN_WIDTH), radius(RADIUS), automaton(nullptr), cursor(nullptr), menu(nullptr),
      nb_frames(0), last_time(0), FPS(""), cell_count(0), seeding(true),
      ready(READY), update_size(false), plague(false), toggle_val(false),
      delta_time(0.f), last_frame(0.f), counter(0), tickrate(TICKRATE) {

    if (path_str.empty()) {
        std::cerr << "Error: Failed to retrieve executable path" << std::endl;
        exit(EXIT_FAILURE);
    }

    init_GLFW();
}

Simulation::~Simulation() {
    // delete automaton;
    automaton = nullptr;

    // delete cursor;
    cursor = nullptr;

    // delete menu;
    menu = nullptr;
}

void Simulation::set_automaton(Automaton *automaton) {
    if (this->automaton != nullptr) {
        delete this->automaton;
    }
    ready = false;
    plague = false;
    this->automaton = automaton;
}

void Simulation::init() {
    automaton = new RND(path_str, window, SQUARE_SIZE);
    cell_count = automaton->get_cell_count();

    // cursor = new Cursor(path_str, win_width, win_height, SQUARE_SIZE,
    // radius);
    cursor = new Cursor(path_str, window, SQUARE_SIZE, radius);
    // automaton->load();
    menu = new Menu(path_str, window);
}

void Simulation::run() {
    std::cout << "Simulation running!" << std::endl;
    while (!glfwWindowShouldClose(window)) {
        counter++;
        process_input();

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        double current_time = glfwGetTime();
        nb_frames++;
        if (current_time - last_time >= 1.0) {
            FPS = std::to_string(nb_frames);

            nb_frames = 0;
            last_time = current_time;
        }

        update_title_bar();

        // glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (update_size) {
            automaton->update_dimensions(win_width, win_height);
            cursor->update_dimensions(win_width, win_height);
            menu->update_dimensions(win_width, win_height);
            cell_count = automaton->get_cell_count();
            update_size = false;
        }

        if (counter > tickrate) {
            if ((!seeding && ready) || step) {
                automaton->update();
                step = false;
            }
            counter = 0;
        }

        automaton->draw();

        menu->draw();
        cursor->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}

std::string Simulation::get_path(char *argv0) {
    char buffer[PATH_MAX];

    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        std::string path = std::string(buffer) + "/" + std::string(argv0);

        char resolved_path[PATH_MAX];
        if (realpath(path.c_str(), resolved_path) != NULL) {
            std::cout << "Resolved bin path: " << resolved_path << std::endl;
            std::string path_str = std::string(resolved_path);

            return std::string(path_str.c_str(),
                               path_str.c_str() + path_str.rfind("/"));
        } else {
            std::cerr << "Realpath error: " << strerror(errno) << std::endl;
            return "";
        }
    } else {
        std::cerr << "Error getting path: " << strerror(errno) << std::endl;
        return "";
    }
}

// std::string Simulation::get_path() { // MacOS only
//     char path[1024];
//     uint32_t size = sizeof(path);
//     if (_NSGetExecutablePath(path, &size) == 0)
//         printf("EP: %s|\n", path);
//     else
//         printf("buffer too small; need size %u\n", size);
//
//     std::string path_str = std::string(path);
//
//     return std::string(path_str.c_str(),
//                        path_str.c_str() + path_str.rfind("/"));
// }

void Simulation::process_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS &&
        (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS ||
         glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)) {
        step = true;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !clicking) {
        bool s = automaton->get_type().find("Sand") != std::string::npos;
        bool w = s & toggle_val;
        bool rps = automaton->get_type().find("RPS") != std::string::npos;
        bool rnd = automaton->get_type().find("RND") != std::string::npos;

        int val = toggle_val ? w ? WATER : SEC : PRIM;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            val = 0;
        }
        if ((s || rps || rnd) && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            val = (rps || rnd) ? SEC + 2 : SOLID;
        }

        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        automaton->set_value(x_pos, y_pos, val, radius, cursor->is_circular());
    } else {
        seeding = false;
    }
}

void Simulation::set_callbacks() {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
                                  int action, int mods) {
        Simulation *instance =
            static_cast<Simulation *>(glfwGetWindowUserPointer(window));
        if (instance)
            instance->key_callback(window, key, scancode, action, mods);
    });
    glfwSetErrorCallback([](int error, const char *description) {
        GLFWwindow *curr_window = glfwGetCurrentContext();
        if (curr_window != nullptr) {
            Simulation *instance = static_cast<Simulation *>(
                glfwGetWindowUserPointer(curr_window));
            if (instance)
                instance->error_callback(error, description);
        } else {
            fprintf(stderr, "Error (No window available): %s\n", description);
        }
    });
    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow *window, int width, int height) {
            Simulation *instance =
                static_cast<Simulation *>(glfwGetWindowUserPointer(window));
            if (instance)
                instance->framebuffer_size_callback(window, width, height);
        });
    glfwSetDropCallback(
        window, [](GLFWwindow *window, int count, const char **paths) {
            Simulation *instance =
                static_cast<Simulation *>(glfwGetWindowUserPointer(window));
            if (instance)
                instance->drop_callback(window, count, paths);
        });
    glfwSetCursorPosCallback(
        window, [](GLFWwindow *window, double x_pos_in, double y_pos_in) {
            Simulation *instance =
                static_cast<Simulation *>(glfwGetWindowUserPointer(window));
            if (instance)
                instance->mouse_pos_callback(window, x_pos_in, y_pos_in);
        });
    glfwSetMouseButtonCallback(
        window, [](GLFWwindow *window, int button, int action, int mods) {
            Simulation *instance =
                static_cast<Simulation *>(glfwGetWindowUserPointer(window));
            if (instance)
                instance->mouse_button_callback(window, button, action, mods);
        });
}

void Simulation::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        int action =  menu->handle_cursor(x_pos, y_pos, true);
        if(action >= 0) clicking = true;
        switch (action) {
            case 0: break;
            case 1:
                set_automaton(
                    new RPS(path_str, window, this->automaton->get_square_size()));
                break;
            case 2:
                set_automaton(
                    new RND(path_str, window, this->automaton->get_square_size()));
                break;
            case 3:
                set_automaton(
                    new Disease(path_str, window, this->automaton->get_square_size()));
                break;
            case 4:
                set_automaton(
                    new LFoD(path_str, window, this->automaton->get_square_size()));
                break;
            case 5:
                set_automaton(
                    new Life(path_str, window, this->automaton->get_square_size()));
                break;
            case 6:
                set_automaton(
                    new Rule0(path_str, window, this->automaton->get_square_size()));
                break;
            case 7:
                set_automaton(
                    new Rule180(path_str, window, this->automaton->get_square_size()));
                break;
            case 8:
                set_automaton(
                    new Rule90(path_str, window, this->automaton->get_square_size()));
                break;
            case 9:
                set_automaton(
                    new Seeds(path_str, window, this->automaton->get_square_size()));
                break;
            case 10:
                set_automaton(
                    new Sand(path_str, window, this->automaton->get_square_size()));
                break;
            default:
                clicking = false;
                break;
        }
    }

}

void Simulation::mouse_pos_callback(GLFWwindow *window, double x_pos_in,
                                    double y_pos_in) {
    float x_pos = static_cast<float>(x_pos_in);
    float y_pos = static_cast<float>(y_pos_in);

    if (x_pos > 0 - SQUARE_SIZE * (radius + 1) &&
        x_pos < win_width + SQUARE_SIZE * (radius + 1) &&
        y_pos > 0 - SQUARE_SIZE * (radius + 1) &&
        y_pos < win_height + SQUARE_SIZE * (radius + 1)) {
        cursor->update_position(x_pos, y_pos);
    }

    // if(x_pos > win_width - BUTTON_WIDTH && x_pos < win_width) {
        menu->handle_cursor(x_pos, y_pos, false);
    // }
}

void Simulation::drop_callback(GLFWwindow *window, int count,
                               const char **paths) {
    for (int i = 0; i < count; i++) {
        std::cout << paths[i] << std::endl;
    }
}

void Simulation::key_callback(GLFWwindow *window, int key, int scancode,
                              int action, int mods) {
    // std::cout << key << std::endl;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        ready = !ready;
    }
    if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS) {
        int val = 1;
        if (mods == GLFW_MOD_SHIFT) {
            val = 10;
        }
        radius += val;
        cursor->update_radius(radius);
    }
    if (key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
        int val = 1;
        if (mods == GLFW_MOD_SHIFT) {
            val = 10;
        }
        radius = std::max(0, radius - val);
        if (radius != cursor->get_radius())
            cursor->update_radius(radius);
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        int val = 10;
        if (mods == GLFW_MOD_SHIFT) {
            val = 100;
        }
        tickrate = std::max(tickrate - val, 0);
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        int val = 10;
        if (mods == GLFW_MOD_SHIFT) {
            val = 100;
        }
        tickrate = std::min(tickrate + val, 1000);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        toggle_val = !toggle_val;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        plague = !plague;
        automaton->toggle_plague();
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        bool all = false;
        if (mods == GLFW_MOD_SHIFT) {
            all = true;
        }
        automaton->clear(all);
        ready = false;
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        // automaton->change_cursor_shape();
        cursor->change_shape();
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        automaton->fill_random();
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        automaton->save();
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        automaton->load();
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        int val = 1;
        if (mods == GLFW_MOD_SHIFT) {
            val = 10;
        }

        int square_size = automaton->get_square_size() + val;

        automaton->update_square_size(square_size);
        int cols = automaton->get_cols();
        if (radius * 2 > cols) {
            radius = cols / 4;
            cursor->update_radius(radius);
        }
        cursor->update_square_size(square_size);
        cell_count = automaton->get_cell_count();
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        int val = 1;
        if (mods == GLFW_MOD_SHIFT) {
            val = 10;
        }
        int square_size = std::max(1, automaton->get_square_size() - val);

        if (square_size != automaton->get_square_size()) {
            automaton->update_square_size(square_size);
            int cols = automaton->get_cols();
            if (radius * 2 > cols) {
                radius = cols / 4;
                cursor->update_radius(radius);
            }
            cursor->update_square_size(square_size);
            cell_count = automaton->get_cell_count();
        }
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        menu->click();
    }
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        step = true;
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        set_automaton(
            new Brain(path_str, window, this->automaton->get_square_size()));
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        set_automaton(
            new DayNNite(path_str, window, this->automaton->get_square_size()));
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        set_automaton(
            new Disease(path_str, window, this->automaton->get_square_size()));
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        set_automaton(
            new LFoD(path_str, window, this->automaton->get_square_size()));
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        set_automaton(
            new Life(path_str, window, this->automaton->get_square_size()));
    }
    if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
        set_automaton(
            new Rule0(path_str, window, this->automaton->get_square_size()));
    }
    if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
        set_automaton(
            new Rule180(path_str, window, this->automaton->get_square_size()));
    }
    if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
        set_automaton(
            new Rule90(path_str, window, this->automaton->get_square_size()));
    }
    if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
        set_automaton(
            new Seeds(path_str, window, this->automaton->get_square_size()));
    }
    if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
        set_automaton(
            new Sand(path_str, window, this->automaton->get_square_size()));
    }
}

void Simulation::framebuffer_size_callback(GLFWwindow *window, int width,
                                           int height) {
    // int aspect_ratio;
    // if(width > height) {
    //     aspect_ratio = width/height;
    // } else {
    //     aspect_ratio = height/width;
    //
    // }
    glViewport(0, 0, width, height);
    glViewport(0, 0, width, height);

    win_width = width;
    win_height = height;

    update_size = true;
}

void Simulation::error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void Simulation::update_title_bar() {
    std::string TITLE_BAR =
        GAME_VERSION_NAME + " - " + FPS +
        " FPS | tickrate: " + std::to_string(tickrate) + " | " +
        std::to_string(cell_count) + " cells" + " | " + automaton->get_type() +
        " | cell size: " + std::to_string(automaton->get_square_size()) + 
        (toggle_val ? "*" : "");

    glfwSetWindowTitle(window, TITLE_BAR.c_str());
}

void Simulation::init_GLFW() {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

    window = glfwCreateWindow(win_width, win_height, GAME_VERSION_NAME.c_str(),
                              NULL, NULL);

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
    glfwSwapInterval(GLFW_FALSE);
    // glfwSwapInterval(GLFW_TRUE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
}
