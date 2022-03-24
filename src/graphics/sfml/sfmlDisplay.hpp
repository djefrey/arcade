/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sfmlDisplay
*/

#pragma once

#include <map>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "arcade-interface/IDisplayModule.hpp"

namespace sfml {
    class SFMLDisplay : public IDisplayModule {
        std::uint32_t _pixelsPerCell = 0;

        std::unique_ptr<sf::RenderWindow> _window;
        sf::Font _font;

    public:
        SFMLDisplay();
        ~SFMLDisplay() = default;

        std::unique_ptr<IDisplayModule::RawTexture> loadTexture(const std::string &pngFilename, char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height) override;

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
}
