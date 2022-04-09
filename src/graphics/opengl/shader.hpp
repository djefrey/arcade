/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** shader
*/

#pragma once

#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace ogl {
    class Shader {
        GLuint _program;

        void compile(GLuint id, const std::string &code);
        void createProgram(GLuint vertex, GLuint fragment);

        public:
        Shader(const std::string &vertexCode, const std::string &fragmentCode);
        ~Shader();

        void bind();
        void unbind();
    };
}