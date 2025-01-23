#ifndef TEXT_HPP
#define TEXT_HPP

#include "shaders.hpp"
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H
#include "glm/gtc/type_ptr.hpp"
#include <glm/glm.hpp>

struct Character {
    unsigned int texture_ID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class Text {
  public:
    Text();
    Text(std::string path_str, GLFWwindow *window);
    void load_font(std::string font, unsigned int font_size);
    void render(std::string text, float x, float y, float scale, glm::vec3 color);
    void render(std::string text, glm::vec2 pos, float scale, glm::vec3 color);

  private:
    std::string path_str;
    int win_width;
    int win_height;
    std::map<char, Character> chars;
    unsigned int VAO, VBO;
    Shader shader_program;
};

#endif
