#include "dropdown_mi.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "src/button_mi.hpp"
#include "src/menu_item.hpp"
#include "util/pxls.hpp"

Dropdown::Dropdown(std::string path_str, int win_width, int win_height,
                   int square_size, int radius)
    : MenuItem(path_str, win_width, win_height, square_size, radius),
       center(0.f), offset(0.f), /* items(1, nullptr), */ collapsed(true) {

    items.push_back(new Button(path_str, win_width, win_height, SQUARE_SIZE, radius));
    // items.push_back(Button(path_str, win_width, win_height, SQUARE_SIZE * 2, radius));
    set_shaders();
}

Dropdown::~Dropdown() {
    glDeleteProgram(shader_program.program_ID); 
    for(auto item : items) {
        delete item;
    }
}

void Dropdown::set_model() {
    std::cout << "dd sm" << std::endl;
    shader_program.use();
    float nx = pxls::to_float(sin(glm::radians(30.f)) * square_size, win_width);
    float ny = pxls::to_float(cos(glm::radians(30.f)) * square_size, win_height);
    center = glm::vec3(nx, ny, 0.f);
    offset = glm::vec3(pxls::to_float(win_width - OFFSET, win_width),
                       pxls::to_float(win_height - OFFSET, win_height), 0);

    model = glm::translate(model, center + 0.5f);
    model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
    shader_program.set_mat4("model", model);
}

void Dropdown::set_vertices() {
    std::cout << "dd sv" << std::endl;
    float vert = pxls::to_float(32, std::max(win_height, win_width));

    quad_vertices = {
        glm::vec2(vert, -vert),
        glm::vec2(-vert, -vert),
        glm::vec2(0, vert),
    };
}

void Dropdown::collapse() {
    std::cout << "collapse" << std::endl;

    model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));
}

void Dropdown::click() {
    collapsed = !collapsed;
    collapsed ? collapse() : expand();

    shader_program.use();
    shader_program.set_mat4("model", model);
}

void Dropdown::expand() {
    std::cout << "expand" << std::endl;

    model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
}

// void Dropdown::set_circle_vertices() {
//     quad_vertices.clear();
//
//     int segments = radius < 60 ? 32 : 64;
//     float angle_step = 2.f * M_PI / segments;
//     int dimensions =
//         radius ? (square_size + GAP) * (radius * 2 + 1) : square_size + GAP;
//
//     for (int i = 0; i < segments; i++) {
//         float x_vert =
//             pxls::to_float(dimensions * cos(angle_step * i) + GAP, win_width);
//         float y_vert =
//             pxls::to_float(dimensions * sin(angle_step * i) + GAP, win_height);
//
//         quad_vertices.push_back(glm::vec2(x_vert, y_vert));
//     }
// }

// void Dropdown::update_position(double x_pos, double y_pos) {
//     cursor_pos = glm::vec2(2 * pxls::to_float(x_pos, win_width) - 1.f,
//                            -2 * pxls::to_float(y_pos, win_height) + 1.f);
//
//     shader_program.set_vec2("cursor_pos", cursor_pos);
// }
//
// void Dropdown::update_square_size(int square_size) {
//     this->square_size = square_size;
//
//     set_shaders();
// }
//
// void Dropdown::update_dimensions(int win_width, int win_height) {
//     this->win_width = win_width;
//     this->win_height = win_height;
//
//     set_shaders();
// }
//
// void Dropdown::update_radius(int radius) {
//     this->radius = radius;
//
//     set_shaders();
// }

void Dropdown::draw() {
    if(!collapsed) {
        if(!items.empty()) {
            for(auto item : items) {
                item->draw();
            }
        }
    }

    shader_program.use();

    glBindVertexArray(quad_VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, quad_vertices.size());
    glBindVertexArray(0);
}
