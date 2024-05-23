#include "mi_radial.hpp"
#include "src/constants.hpp"
#include "util/glm_colors.hpp"
#include "util/pxls.hpp"

Radial::Radial(std::string path_str, GLFWwindow *window, int size,
                   glm::vec3 position, glm::vec3 color)
    : MenuItem(path_str, window, position, color, -1, -1.f, false), center(0.f), collapsed(true),
      size(size) {

    float nx = pxls::to_float(sin(glm::radians(30.f)) * size, win_width);
    float ny = pxls::to_float(cos(glm::radians(30.f)) * size, win_height);
    center = glm::vec3(nx, ny, 0.f);

    // float button_gap = pxls::to_float(BUTTON_HEIGHT * 3, win_height);
    std::vector<glm::vec3> colors = {
        purple, green, light_blue, red, green,
        blue, purple, red, orange, sand_yellow
    };

    for (int i = 0; i < 10; i++) {
        glm::vec3 button_pos = position + determine_position(i);
        int angle = i * 36;

        items.push_back(new Button( path_str, window, BUTTON_WIDTH, BUTTON_HEIGHT, button_pos, colors[i], i + 1, angle));
    }

    set_shaders();
}

glm::vec3 Radial::determine_position(int idx){
    float nx = pxls::to_float(sin(glm::radians((float) ((idx * 36)))) * BUTTON_WIDTH * 3, win_width);
    float ny = pxls::to_float(cos(glm::radians((float) ((idx * 36)))) * BUTTON_WIDTH * 3, win_height);
    // std::cout << nx << std::endl;
    // center = glm::vec3(nx, ny, 0.f);

    return center - glm::vec3(nx, ny, 0.f);
}

void Radial::update_dimensions(int win_width, int win_height) {
    this->win_width = win_width;
    this->win_height = win_height;

    for (auto item : items) {
        item->update_dimensions(win_width, win_height);
    }

    set_shaders();
}

Radial::~Radial() {
    glDeleteProgram(shader_program.program_ID);

    for (auto item : items) {
        delete item;
    }
}

void Radial::set_model() {
    shader_program.use();

    model = glm::translate(model, center + position);
    if(!collapsed)
        model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
    shader_program.set_mat4("model", model);
}

void Radial::set_vertices() {
    float vert =
        pxls::to_float(DROPDOWN_ARROW_SIZE, std::max(win_height, win_width));

    quad_vertices = {
        glm::vec2(vert, -vert),
        glm::vec2(-vert, -vert),
        glm::vec2(0, vert),
    };
}

int Radial::handle_cursor(double x_pos, double y_pos, bool clicking) {
    glm::vec2 vert0 = quad_vertices[0] + glm::vec2(center.x + position.x, center.y + position.y); //bot right
    glm::vec2 vert1 = quad_vertices[1] + glm::vec2(center.x + position.x, center.y + position.y); //bot left
    glm::vec2 vert2 = quad_vertices[2] + glm::vec2(center.x + position.x, center.y + position.y); //top

    float v0_x = win_width * ((vert0.x) + 1.f) / 2;
    float v0_y = win_height * (1.f - (vert0.y)) / 2;
    float v1_x = win_width * ((vert1.x) + 1.f) / 2;
    float v2_y = win_height * (1.f - (vert2.y)) / 2;

    int action = -1;

    if(x_pos <= v0_x && y_pos <= v0_y
        && x_pos >= v1_x
        && y_pos >= v2_y

    ) {
        if(clicking) {
            action = 0;
            click();
        } else {
            hover();
        }
    }

    if(!collapsed) {
        for(const auto item : items) {
            int action = item->handle_cursor(x_pos, y_pos, clicking);
            if(action > 0) {
                return action;
            }
        }
    }

    return action;
}

void Radial::collapse() {
    std::cout << "collapse" << std::endl;

    size /= 2;
    model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));
}

void Radial::click() {
    collapsed = !collapsed;
    collapsed ? size /= 2 : size *= 2;
    set_shaders();

    model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
    shader_program.use();
    shader_program.set_mat4("model", model);
}

void Radial::expand() {
    std::cout << "expand" << std::endl;

    model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
}

void Radial::set_circular_vertices() {
    quad_vertices.clear();

    int segments = size < 20 ? 32 : 64;
    float angle_step = 2.f * M_PI / segments;
    int dimensions = size;

    for (int i = 0; i < segments; i++) {
        float x_vert =
            pxls::to_float(dimensions * cos(angle_step * i) + GAP,
            win_width);
        float y_vert =
            pxls::to_float(dimensions * sin(angle_step * i) + GAP,
            win_height);

        quad_vertices.push_back(glm::vec2(x_vert, y_vert));
    }
}

// void Radial::update_position(double x_pos, double y_pos) {
//     cursor_pos = glm::vec2(2 * pxls::to_float(x_pos, win_width) - 1.f,
//                            -2 * pxls::to_float(y_pos, win_height) + 1.f);
//
//     shader_program.set_vec2("cursor_pos", cursor_pos);
// }
//
// void Radial::update_square_size(int square_size) {
//     this->square_size = square_size;
//
//     set_shaders();
// }
//
// void Radial::update_dimensions(int win_width, int win_height) {
//     this->win_width = win_width;
//     this->win_height = win_height;
//
//     set_shaders();
// }
//
// void Radial::update_radius(int radius) {
//     this->radius = radius;
//
//     set_shaders();
// }

void Radial::draw() {
    if (!collapsed) {
        if (!items.empty()) {
            for (auto item : items) {
                item->draw();
            }
        }
    }

    shader_program.use();

    glBindVertexArray(quad_VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, quad_vertices.size());
    glBindVertexArray(0);
}
