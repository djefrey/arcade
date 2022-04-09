/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** png
*/

#include "texture.hpp"
#include "png.hpp"
#include "font.hpp"
#include <iostream>

ogl::OpenGLTexture::OpenGLTexture(const std::string &filename, uint width, uint height) :
    _width(width), _height(height)
{
    uint textWidth;
    uint textHeight;
    uint8_t *data;

    if (textWidth == 0 || textHeight == 0)
        return;
    data = PNG::readPNGFile(filename, &textWidth, &textHeight);
    glGenTextures(1, &_textureId);
    this->bind();
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, textWidth, textHeight);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textWidth, textHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    this->unbind();
    delete data;
}

ogl::OpenGLTexture::OpenGLTexture(const std::string &fontpath, char c, uint size, Color textColor, Color bkgdColor) :
    _width(size), _height(size)
{
    OGLColor oglTextColor = OPENGL_COLORS.at(textColor);
    OGLColor oglBkgdColor = OPENGL_COLORS.at(bkgdColor);
    FontCharData fontData = OpenGLFont::get().getFontBitmap(fontpath, c, oglTextColor, oglBkgdColor);
    uint8_t *data = std::get<0>(fontData);
    uint width = std::get<1>(fontData);
    uint height = std::get<2>(fontData);

    if (width == 0 || height == 0)
        return;
    glGenTextures(1, &_textureId);
    this->bind();
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    this->unbind();
    delete data;
}

ogl::OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &_textureId);
}

void ogl::OpenGLTexture::bind()
{
    glBindTexture(GL_TEXTURE_2D, _textureId);
}

void ogl::OpenGLTexture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
