#include "menu.hpp"

Menu::Menu(std::string path_str, GLFWwindow *window) {
    glfwGetWindowSize(window, &win_width, &win_height);
    glm::vec3 offset = glm::vec3(pxls::to_float(win_width - OFFSET, win_width),
                       pxls::to_float(win_height - OFFSET, win_height), 0);
    glm::vec3 opposite_offset = glm::vec3(pxls::to_float(-(win_width - OFFSET), win_width),
                       pxls::to_float(win_height - OFFSET, win_height), 0);
    items.push_back(new Dropdown(path_str, window, DROPDOWN_ARROW_SIZE, offset));
    items.push_back(new Radial(path_str, window, DROPDOWN_ARROW_SIZE, opposite_offset));

}

Menu::~Menu() {
    for(const auto item : items) {
        delete item;
    }
}

void Menu::update_dimensions(int win_width, int win_height) {
    for(const auto item : items) {
        item->update_dimensions(win_width, win_height);
    }
}


int Menu::handle_cursor(double x_pos, double y_pos, bool clicking) {
    for(const auto item : items) {
        int action = item->handle_cursor(x_pos, y_pos, clicking);
        if(action >= 0) {
            return action;
        }
    }

    return -1;
}

void Menu::click() {
    for(const auto item : items) {
        item->click();
    }

}

void Menu::draw() {
    for(const auto item : items) {
        item->draw();
    }
}
