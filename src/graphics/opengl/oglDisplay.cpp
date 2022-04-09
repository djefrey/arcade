/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** oglDisplay
*/

#include <X11/Xlib.h>
#undef None

#include <stdexcept>
#include <filesystem>
#include "oglDisplay.hpp"

ogl::OpenGLDisplay::OpenGLDisplay()
{
    XInitThreads();
    glewInit();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenBuffers(1, &_buffer);
    _shader = std::make_unique<Shader>(VERTEX_SHADER, FRAGMENT_SHADER);
}

ogl::OpenGLDisplay::~OpenGLDisplay()
{
    _window.close();
}

void ogl::OpenGLDisplay::openWindow(Vector2u size)
{
    sf::VideoMode mode{size.x, size.y};
    sf::ContextSettings settings{0, 0, 0, 3, 3, sf::ContextSettings::Default, false};

    _window.create(mode, "Arcade (OpenGL)", 7U, settings);
    _windowSize = size;
}

void ogl::OpenGLDisplay::display()
{
    _window.display();
}

void ogl::OpenGLDisplay::clearScreen(IDisplayModule::Color color)
{
    OGLColor glColor = OPENGL_COLORS.at(color);

    glClearColor(glColor.r, glColor.g, glColor.b, glColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

std::unique_ptr<IDisplayModule::RawTexture> ogl::OpenGLDisplay::loadTexture(const std::string &filename, char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height)
{
    std::filesystem::path filenamePath{filename};

    (void) height;
    if (filenamePath.extension() == ".png")
        return std::make_unique<OpenGLTexture>(filename, width, height);
    if (filenamePath.extension() == ".ttf")
        return std::make_unique<OpenGLTexture>(filename, character, width, characterColor, backgroundColor);
    throw std::runtime_error("Tried to load texture from invalid file !");
}

#include <iostream>

void ogl::OpenGLDisplay::renderSprite(IDisplayModule::Sprite sprite)
{
    ogl::OpenGLTexture *texture = dynamic_cast<ogl::OpenGLTexture*>(sprite.texture);
    Vector2u size = texture->getSize();

    GLfloat startX = ((float) sprite.rawPixelPosition.x) / _windowSize.x * 2 - 1;
    GLfloat startY = ((float) sprite.rawPixelPosition.y) / _windowSize.y * -2 + 1;
    GLfloat sizeX = ((float) size.x / _windowSize.x) * 2;
    GLfloat sizeY = ((float) size.y / _windowSize.y) * 2;
    GLfloat data[4 * 4] = {
    //         X               Y         U    V
        startX,         startY,         0.0, 1.0, // 0
        startX,         startY + sizeY, 0.0, 0.0, // 1
        startX + sizeX, startY,         1.0, 1.0, // 2
        startX + sizeX, startY + sizeY, 1.0, 0.0, // 3
    };

    _shader->bind();
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_DYNAMIC_DRAW);
    texture->bind();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    texture->unbind();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    _shader->unbind();
}

sf::Font &ogl::OpenGLDisplay::getFont(const std::string &fontFilename)
{
    auto [result, didInsert] = _fonts.emplace(fontFilename, sf::Font());
    if (didInsert && !result->second.loadFromFile(fontFilename))
        throw std::runtime_error("Could not load font file '" + fontFilename + "'");
    return result->second;
}

void ogl::OpenGLDisplay::update()
{
    sf::Event event;

    _leftMouseRelease = false;
    _rightMouseRelease = false;
    _textInput.clear();
    _buttonsPressedThisFrame.clear();
    while (_window.pollEvent(event)) {
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

bool ogl::OpenGLDisplay::isButtonPressed(Button button)
{
    auto it = _buttonsPressedThisFrame.find(button);
    return it == _buttonsPressedThisFrame.end() ? false : it->second;
}

IDisplayModule::MouseButtonReleaseEvent ogl::OpenGLDisplay::getMouseButtonReleaseEvent()
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
