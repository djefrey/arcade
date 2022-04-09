/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** font
*/

#pragma once

#include <memory>
#include <unordered_map>
#include "oglDisplay.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace ogl {
    using FontCharData = std::tuple<uint8_t*, uint, uint>;

    class OpenGLFont {
        static std::unique_ptr<OpenGLFont> _instance;

        FT_Library lib;
        std::unordered_map<std::string, FT_Face> _fonts;

        OpenGLFont();
        FT_Face getFontFace(const std::string &filepath);

        public:
        ~OpenGLFont();

        static OpenGLFont &get()
        {
            if (_instance == nullptr)
                _instance.reset(new OpenGLFont);
            return *_instance.get();
        };

        FontCharData getFontBitmap(const std::string &filepath, char c, OGLColor textColor, OGLColor bkgdColor);
    };
}

