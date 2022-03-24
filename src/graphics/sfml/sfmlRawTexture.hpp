/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sfmlRawTexture
*/

#pragma once

#include "arcade-interface/IDisplayModule.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Font.hpp>

namespace sfml {
    class SFMLRawTexture : public IDisplayModule::RawTexture {
    public:
        virtual ~SFMLRawTexture() = default;

        virtual const sf::Texture &getTexture() const = 0;
    };

    class SFMLRawGraphicTexture : public SFMLRawTexture {
        sf::Texture _texture;

        public:
            SFMLRawGraphicTexture(const std::string &pngFilename);
            ~SFMLRawGraphicTexture() = default;

            const sf::Texture &getTexture() const;
    };

    class SFMLRawASCIITexture : public SFMLRawTexture {
        sf::RenderTexture _texture;

        public:
            SFMLRawASCIITexture(char character, IDisplayModule::Color charColor, IDisplayModule::Color bkgdColor, unsigned int characterSize, const sf::Font &font);
            ~SFMLRawASCIITexture() = default;

            const sf::Texture &getTexture() const;
    };
}
