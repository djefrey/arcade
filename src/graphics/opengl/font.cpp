/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** font
*/

#include <stdexcept>
#include <string.h>
#include "font.hpp"

std::unique_ptr<ogl::OpenGLFont> ogl::OpenGLFont::_instance = nullptr;

ogl::OpenGLFont::OpenGLFont()
{
    if (FT_Init_FreeType(&lib))
        throw std::runtime_error("Could not init FreeType");
};

ogl::OpenGLFont::~OpenGLFont()
{
    for (std::pair<std::string, FT_Face> pair : _fonts)
        FT_Done_Face(pair.second);
}

FT_Face ogl::OpenGLFont::getFontFace(const std::string &filepath)
{
    FT_Face face;

    if (_fonts.find(filepath) == _fonts.end()) {
        if (FT_New_Face(lib, filepath.c_str(), 0, &face))
            throw std::runtime_error("Could not load fond " + filepath);
        _fonts[filepath] = face;
    }
    return _fonts.at(filepath);
}

#include <iostream>

ogl::FontCharData ogl::OpenGLFont::getFontBitmap(const std::string &filepath, char c, OGLColor textColor, OGLColor bkgdColor)
{
    const uint8_t textColor8b[4] = {(uint8_t) textColor.r, (uint8_t) textColor.g, (uint8_t) textColor.b, (uint8_t) textColor.a};
    const uint8_t bkgdColor8b[4] =  {(uint8_t) bkgdColor.r, (uint8_t) bkgdColor.g, (uint8_t) bkgdColor.b, (uint8_t) bkgdColor.a};
    const FT_Face face = getFontFace(filepath);
    uint8_t *data;
    uint8_t *bitmap;

    FT_Set_Pixel_Sizes(face, 0, 24);
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        throw std::runtime_error(std::string("Could not load glyph for char '") + c + "'");
    bitmap = face->glyph->bitmap.buffer;
    data = new uint8_t[4 * face->glyph->bitmap.width * face->glyph->bitmap.rows];
    for (int i = 0; i < face->glyph->bitmap.width * face->glyph->bitmap.rows; i++) {
        if (bitmap[i] == 0)
            memcpy(data + i * 4, bkgdColor8b, 4);
        else
            memcpy(data + i * 4, textColor8b,  4);
    }
    return std::make_tuple(data, face->glyph->bitmap.width, face->glyph->bitmap.rows);
};
