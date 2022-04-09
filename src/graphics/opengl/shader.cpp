/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** shader
*/

#include <stdexcept>
#include "shader.hpp"

ogl::Shader::Shader(const std::string &vertexCode, const std::string &fragmentCode)
{
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    compile(vertex, vertexCode);
    compile(fragment, fragmentCode);
    createProgram(vertex, fragment);
    glDeleteShader(fragment);
    glDeleteShader(vertex);
}

ogl::Shader::~Shader()
{
    glDeleteProgram(_program);
}

void ogl::Shader::compile(GLuint id, const std::string &code)
{
    int success;
    const char *cCode = code.c_str();
    char infoLog[512];

    glShaderSource(id, 1, (const char * const *) &cCode, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        throw std::runtime_error(std::string("Error while compiling shader : ") + infoLog);
    }
}

void ogl::Shader::createProgram(GLuint vertex, GLuint fragment)
{
    int success;

    _program = glCreateProgram();
    glAttachShader(_program, vertex);
    glAttachShader(_program, fragment);
    glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (!success)
        throw std::runtime_error("Error while creating shader program");
    glDetachShader(_program, fragment);
    glDetachShader(_program, vertex);
}

void ogl::Shader::bind()
{
    glUseProgram(_program);
}

void ogl::Shader::unbind()
{
    glUseProgram(0);
}
