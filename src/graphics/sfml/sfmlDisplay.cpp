/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sfmlDisplay
*/

#include <SFML/Graphics.hpp>
#include "sfmlDisplay.hpp"
#include "sfmlRawTexture.hpp"

sfml::SFMLDisplay::SFMLDisplay()
{
    if (!_font.loadFromFile("./assets/font/Joystix.TTF"))
        throw std::runtime_error("Could not load font file");
}

void sfml::SFMLDisplay::openWindow(Vector2u size)
{
    sf::VideoMode mode(size.x, size.y);

    _window = std::make_unique<sf::RenderWindow>(mode, "Arcade");
}

std::unique_ptr<IDisplayModule::RawTexture> sfml::SFMLDisplay::loadTexture(const std::string &pngFilename, char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height)
{
    if (pngFilename.size() > 0)
        return std::make_unique<SFMLRawGraphicTexture>(pngFilename);
    else
        return std::make_unique<SFMLRawASCIITexture>(character, characterColor, backgroundColor, _pixelsPerCell, _font);
}

void sfml::SFMLDisplay::clearScreen(IDisplayModule::Color color)
{
    _window->clear(SFML_COLORS.at(color));
}

void sfml::SFMLDisplay::renderSprite(IDisplayModule::Sprite sprite)
{
    sfml::SFMLRawTexture *sfmlTexture = dynamic_cast<sfml::SFMLRawTexture*>(sprite.texture);
    sf::Sprite sfmlSprite{sfmlTexture->getTexture()};

    sfmlSprite.setPosition(sf::Vector2f(sprite.rawPixelPosition.x, sprite.rawPixelPosition.y));
    _window->draw(sfmlSprite);
}

void sfml::SFMLDisplay::display()
{
    _window->display();
}

void sfml::SFMLDisplay::update()
{
    sf::Event event;
    sf::Keyboard::Key key;

    _leftMouseRelease = false;
    _rightMouseRelease = false;
    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _close = true;
        else if (event.type == sf::Event::TextEntered)
            _textInput += event.text.unicode;
        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left)
                _leftMouseRelease = true;
            else if (event.mouseButton.button == sf::Mouse::Right)
                _rightMouseRelease = true;
        } else if (event.type == sf::Event::MouseMoved)
            _mousePos = (Vector2u) {event.mouseButton.x / _pixelsPerCell, event.mouseButton.y / _pixelsPerCell};
    }
}

bool sfml::SFMLDisplay::isButtonPressed(Button button)
{
    return sf::Keyboard::isKeyPressed(SFML_BUTTONS.at(button));
}

IDisplayModule::MouseButtonReleaseEvent sfml::SFMLDisplay::getMouseButtonReleaseEvent()
{
    MouseButtonReleaseEvent ev;

    if (_leftMouseRelease)
        ev.type = MouseButtonReleaseEvent::Type::Left;
    else if (_rightMouseRelease)
        ev.type = MouseButtonReleaseEvent::Type::Right;
    else
        ev.type = MouseButtonReleaseEvent::Type::None;
    ev.cellPosition = _mousePos;
    return ev;
}

bool sfml::SFMLDisplay::isClosing()
{
    return _close;
}

void sfml::SFMLDisplay::startTextInput()
{
    _textInput.clear();
    _readTextInput = true;
}

std::string sfml::SFMLDisplay::getTextInput()
{
    return _textInput;
}

void sfml::SFMLDisplay::endTextInput()
{
    _textInput.clear();
    _readTextInput = false;
}

void sfml::SFMLDisplay::setPixelsPerCell(std::uint32_t pixelsPerCell)
{
    this->_pixelsPerCell = pixelsPerCell;
}

std::uint32_t sfml::SFMLDisplay::getPixelsPerCell()
{
    return this->_pixelsPerCell;
}