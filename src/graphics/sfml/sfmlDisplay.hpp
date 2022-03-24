/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sfmlDisplay
*/

#pragma once

#include <map>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "arcade-interface/IDisplayModule.hpp"
#include <optional>

namespace sfml {
    class SFMLDisplay : public IDisplayModule {
        std::uint32_t _pixelsPerCell = 0;

        std::unique_ptr<sf::RenderWindow> _window;
        std::unordered_map<std::string, sf::Font> _fonts;

        bool _close = false;
        bool _leftMouseRelease = false;
        bool _rightMouseRelease = false;
        Vector2u _mousePos = {0, 0};

        bool _readTextInput = false;
        std::string _textInput;

        sf::Font &getFont(const std::string &fontFilename);

    public:
        SFMLDisplay();
        ~SFMLDisplay() = default;

        std::unique_ptr<IDisplayModule::RawTexture> loadTexture(const std::string &filename, char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height) override;

        void openWindow(IDisplayModule::Vector2u pixelsWantedWindowSize) override;

        bool isButtonPressed(IDisplayModule::Button button) override;
        IDisplayModule::MouseButtonReleaseEvent getMouseButtonReleaseEvent() override;
        bool isClosing() override;

        void startTextInput() override;
        std::string getTextInput() override;
        void endTextInput() override;

        void clearScreen(IDisplayModule::Color color) override;
        void renderSprite(IDisplayModule::Sprite sprite) override;
        void display() override;
        void update() override;

        void setPixelsPerCell(std::uint32_t pixelsPerCell) override;
        std::uint32_t getPixelsPerCell() override;
    };

    extern const std::map<IDisplayModule::Color, sf::Color> SFML_COLORS;
    extern const std::map<IDisplayModule::Button, sf::Keyboard::Key> SFML_BUTTONS;
}
