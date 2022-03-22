/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sfmlDisplay
*/

#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "arcade-interface/IDisplayModule.hpp"

namespace sfml {
    class SFMLDisplay : public IDisplayModule {
        std::uint32_t _pixelsPerCell;

    public:
        SFMLDisplay() = default;
        ~SFMLDisplay() = default;

        void setPixelsPerCell(std::uint32_t pixelsPerCell);
        std::uint32_t getPixelsPerCell();

        class SFMLRawTexture : public RawTexture {
        public:
            SFMLRawTexture();
            ~SFMLRawTexture();
        };

        std::unique_ptr<IDisplayModule::RawTexture> loadTexture(const std::string &pngFilename, char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height);

        void openWindow(IDisplayModule::Vector2u pixelsWantedWindowSize);

        bool isButtonPressed(IDisplayModule::Button button);
        IDisplayModule::MouseButtonReleaseEvent getMouseButtonReleaseEvent();

        void startTextInput();
        std::string getTextInput();
        void endTextInput();

        void clearScreen(IDisplayModule::Color color);
        void renderSprite(IDisplayModule::Sprite sprite);
        void display();
    };
}
