/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sfmlDisplay
*/

#include <SFML/Graphics.hpp>
#include "sfmlDisplay.hpp"

void sfml::SFMLDisplay::setPixelsPerCell(std::uint32_t pixelsPerCell)
{
    this->_pixelsPerCell = pixelsPerCell;
}

std::uint32_t sfml::SFMLDisplay::getPixelsPerCell()
{
    return this->_pixelsPerCell;
}

std::unique_ptr<IDisplayModule::RawTexture> loadTexture(const std::string &pngFilename, char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height)
{
    return std::make_unique<sfml::SFMLDisplay::SFMLRawTexture>();
}

void sfml::SFMLDisplay::openWindow(Vector2u size)
{

}

bool sfml::SFMLDisplay::isButtonPressed(Button button)
{
    return false;
}

IDisplayModule::MouseButtonReleaseEvent sfml::SFMLDisplay::getMouseButtonReleaseEvent()
{
    return MouseButtonReleaseEvent();
}

void sfml::SFMLDisplay::startTextInput()
{

}

std::string sfml::SFMLDisplay::getTextInput()
{
    return "TO BE IMPLEMENTED";
}

void sfml::SFMLDisplay::endTextInput()
{

}

void sfml::SFMLDisplay::renderSprite(IDisplayModule::Sprite sprite)
{

}

void sfml::SFMLDisplay::display()
{

}
