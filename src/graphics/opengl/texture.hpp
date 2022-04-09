/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** texture
*/

#pragma once

#include <string>
#include <GL/glew.h>
#include "arcade-interface/IDisplayModule.hpp"

namespace ogl {
    class OpenGLTexture : public virtual IDisplayModule::RawTexture {
        using Color = IDisplayModule::Color;
        uint _width = 0;
        uint _height = 0;
        GLuint _textureId = 0;

        public:
        OpenGLTexture(const std::string &filename, uint width, uint height);
        OpenGLTexture(const std::string &fontpath, char c, uint size, Color textColor, Color bkgdColor);
        ~OpenGLTexture();

        void bind();
        void unbind();
        IDisplayModule::Vector2u getSize() { return {_width, _height}; };
    };
}