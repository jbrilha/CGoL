#include "menu.hpp"

Menu::Menu(std::string path_str, int win_width, int win_height,
               int square_size, int radius) {

    items.push_back(new Dropdown(path_str, win_width, win_height, SQUARE_SIZE, radius));
}

Menu::~Menu() {
    for(auto item : items) {
        delete item;
    }
}

void Menu::update_dimensions(int win_width, int win_height) {
}


void Menu::draw() {
    for(auto item : items) {
        item->draw();
    }
}
