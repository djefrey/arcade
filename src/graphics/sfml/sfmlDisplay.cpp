/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sfmlDisplay
*/

#include <SFML/Graphics.hpp>
#include "sfmlDisplay.hpp"
#include "sfmlRawTexture.hpp"
#include <filesystem>
#include <stdexcept>
#include <X11/Xlib.h>
#undef None // wtf is X11 smoking with these bullshit #defines

sfml::SFMLDisplay::SFMLDisplay()
{
    XInitThreads(); // This is to fix a quite complicated bug (which took me so long to solve, gods...). See https://github.com/SFML/SFML/issues/2060 for more details on this.
}

void sfml::SFMLDisplay::openWindow(Vector2u size)
{
    _window = std::make_unique<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Arcade");
}

sf::Font &sfml::SFMLDisplay::getFont(const std::string &fontFilename)
{
    auto [result, didInsert] = _fonts.emplace(fontFilename, sf::Font());
    if (didInsert && !result->second.loadFromFile(fontFilename))
        throw std::runtime_error("Could not load font file '" + fontFilename + "'");
    return result->second;
}

std::unique_ptr<IDisplayModule::RawTexture> sfml::SFMLDisplay::loadTexture(const std::string &filename, char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height)
{
    (void)height;

    std::filesystem::path filenamePath{filename};
    if (filenamePath.extension() == ".png")
        return std::make_unique<SFMLRawGraphicTexture>(filename);
    if (filenamePath.extension() == ".ttf")
        return std::make_unique<SFMLRawASCIITexture>(character, characterColor, backgroundColor, width, this->getFont(filename));
    throw std::runtime_error("Tried to load texture from invalid file !");
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

    _leftMouseRelease = false;
    _rightMouseRelease = false;
    _textInput.clear();
    _buttonsPressedThisFrame.clear();
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
            _mousePos = (Vector2u) {event.mouseButton.x / _pixelsPerCell, event.mouseButton.y / _pixelsPerCell};
        } else if (event.type == sf::Event::KeyPressed) {
            auto displayButton = SFML_BUTTONS.find(event.key.code);

            if (displayButton != SFML_BUTTONS.end())
                _buttonsPressedThisFrame[displayButton->second] = true;
        }
    }
}

bool sfml::SFMLDisplay::isButtonPressed(Button button)
{
    auto it = _buttonsPressedThisFrame.find(button);
    return it == _buttonsPressedThisFrame.end() ? false : it->second;
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
    _readTextInput = true;
}

std::string sfml::SFMLDisplay::getTextInput()
{
    return _textInput;
}

void sfml::SFMLDisplay::endTextInput()
{
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
