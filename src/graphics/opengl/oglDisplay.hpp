/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** oglDisplay
*/

#pragma once

#include <unordered_map>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "arcade-interface/IDisplayModule.hpp"
#include "texture.hpp"
#include "shader.hpp"

namespace ogl {
    struct OGLColor {
        GLclampf r;
        GLclampf g;
        GLclampf b;
        GLclampf a;
    };

    class OpenGLDisplay : public virtual IDisplayModule {
        using Vector2u = IDisplayModule::Vector2u;

        std::uint32_t _pixelsPerCell;

        sf::Window _window;
        Vector2u _windowSize;

        std::unordered_map<std::string, sf::Font> _fonts;
        std::unordered_map<IDisplayModule::Button, bool> _buttonsPressedThisFrame;

        GLuint _buffer;
        std::unique_ptr<Shader> _shader;

        bool _close = false;

        bool _leftMouseRelease = false;
        bool _rightMouseRelease = false;
        Vector2u _mousePos = {0, 0};

        std::string _textInput = "";

        sf::Font &getFont(const std::string &fontFilename);

        public:
        OpenGLDisplay();
        ~OpenGLDisplay();

        void openWindow(IDisplayModule::Vector2u pixelsWantedWindowSize);

        void update();

        bool isButtonPressed(IDisplayModule::Button button);
        IDisplayModule::MouseButtonReleaseEvent getMouseButtonReleaseEvent();
        bool isClosing() { return _close; };
        void startTextInput() {};
        std::string getTextInput() { return _textInput; };
        void endTextInput() {};

        void display();
        void clearScreen(IDisplayModule::Color color);
        std::unique_ptr<IDisplayModule::RawTexture> loadTexture(const std::string &filename, char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height);
        void renderSprite(IDisplayModule::Sprite sprite);

        void setPixelsPerCell(std::uint32_t pixelsPerCell) { _pixelsPerCell = pixelsPerCell; };
        std::uint32_t getPixelsPerCell() { return _pixelsPerCell; };
    };

    extern const std::unordered_map<IDisplayModule::Color, OGLColor> OPENGL_COLORS;
    extern const std::unordered_map<IDisplayModule::Color, sf::Color> SFML_COLORS;
    extern const std::unordered_map<sf::Keyboard::Key, IDisplayModule::Button> SFML_BUTTONS;
    extern const std::string VERTEX_SHADER;
    extern const std::string FRAGMENT_SHADER;
}