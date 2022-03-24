/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sfmlRawTexture
*/

#include <stdexcept>
#include <SFML/Graphics/Text.hpp>
#include "sfmlDisplay.hpp"
#include "sfmlRawTexture.hpp"

sfml::SFMLRawGraphicTexture::SFMLRawGraphicTexture(const std::string &pngFilename) : _texture{}
{
    if (!_texture.loadFromFile(pngFilename))
        throw std::runtime_error("Could not load texture " + pngFilename);
}

sfml::SFMLRawASCIITexture::SFMLRawASCIITexture(char character, IDisplayModule::Color charColor, IDisplayModule::Color bkgdColor, unsigned int _pixelsPerCell, const sf::Font &font) : _texture{}
{
    char str[2] = {character, '\0'};
    sf::Text text{std::string(str), font, _pixelsPerCell};

    if (!_texture.create(_pixelsPerCell, _pixelsPerCell))
        throw std::runtime_error("Could not create render texture");
    text.setFillColor(SFML_COLORS.at(charColor));
    text.setPosition(sf::Vector2f(0, 0));
    _texture.clear(SFML_COLORS.at(bkgdColor));
    _texture.draw(text);
    _texture.display();
}

const sf::Texture &sfml::SFMLRawGraphicTexture::getTexture() const
{
    return _texture;
}

const sf::Texture &sfml::SFMLRawASCIITexture::getTexture() const
{
    return _texture.getTexture();
}