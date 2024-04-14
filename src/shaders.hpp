#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

class Shader {
  public:
    unsigned int program_ID;

    Shader() {};
    Shader(const char *vert_path, const char *frag_path) {
        std::string vert_code;
        std::string frag_code;
        std::ifstream vert_shader_file;
        std::ifstream frag_shader_file;

        vert_shader_file.exceptions(std::ifstream::failbit |
                                    std::ifstream::badbit);
        frag_shader_file.exceptions(std::ifstream::failbit |
                                    std::ifstream::badbit);

        try {
            vert_shader_file.open(vert_path);
            frag_shader_file.open(frag_path);

            std::stringstream vert_shader_stream, frag_shader_stream;

            vert_shader_stream << vert_shader_file.rdbuf();
            frag_shader_stream << frag_shader_file.rdbuf();

            vert_shader_file.close();
            frag_shader_file.close();

            vert_code = vert_shader_stream.str();
            frag_code = frag_shader_stream.str();

        } catch (std::ifstream::failure e) {
            std::cout << "ERROR:SHADERS::FILES_FAILED_TO_READ" << std::endl;
        }

        const char *vert_shader_code = vert_code.c_str();
        const char *frag_shader_code = frag_code.c_str();

        // shdaer compilation
        const unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &vert_shader_code, NULL);
        glCompileShader(vert_shader);
        check_for_errors(vert_shader, "VERTEX");

        const unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &frag_shader_code, NULL);
        glCompileShader(frag_shader);
        check_for_errors(frag_shader, "FRAGMENT");

        program_ID = glCreateProgram();
        glAttachShader(program_ID, vert_shader);
        glAttachShader(program_ID, frag_shader);
        glLinkProgram(program_ID);
        check_for_errors(program_ID, "PROGRAM");

        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
    }

    void use() { glUseProgram(program_ID); }

    void del() { glDeleteProgram(program_ID); }

    void set_bool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(program_ID, name.c_str()), (int)value);
    }

    void set_int(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(program_ID, name.c_str()), value);
    }

    void set_float(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(program_ID, name.c_str()), value);
    }

    void set_vec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(program_ID, name.c_str()), x, y);
    }

    void set_vec2(const std::string &name, glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(program_ID, name.c_str()), 1,
                     &value[0]);
    }

    // void set_vec3(const std::string &name, float x, float y, float z) const {
    //     glUniform3f(glGetUniformLocation(program_ID, name.c_str()), x, y, z);
    // }

    void set_vec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(program_ID, name.c_str()), 1,
                     &value[0]);
    }

    void set_vec4(const std::string &name, float x, float y, float z,
                  float w) const {
        glUniform4f(glGetUniformLocation(program_ID, name.c_str()), x, y, z, w);
    }

    void set_vec4(const std::string &name, glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(program_ID, name.c_str()), 1,
                     &value[0]);
    }

    void set_mat2(const std::string &name, glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(program_ID, name.c_str()), 1,
                           GL_FALSE, &mat[0][0]);
    }

    void set_mat3(const std::string &name, glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(program_ID, name.c_str()), 1,
                           GL_FALSE, &mat[0][0]);
    }

    void set_mat4(const std::string &name, glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(program_ID, name.c_str()), 1,
                           GL_FALSE, &mat[0][0]);
    }

  private:
    void check_for_errors(unsigned int ID, std::string type) {
        int success;
        char info_log[1024];

        if (type != "PROGRAM") {
            glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(ID, 1024, NULL, info_log);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                          << info_log << std::endl;
            }
        } else {
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(ID, 1024, NULL, info_log);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                          << info_log << std::endl;
            }
        }
    }
};

#endif // !SHADERS_H
