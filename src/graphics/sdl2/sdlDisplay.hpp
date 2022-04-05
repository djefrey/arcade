/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sdlDisplay
*/

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <map>
#include "arcade-interface/IDisplayModule.hpp"

namespace sdl {
    class SDLDisplay : public IDisplayModule {
        std::uint32_t _pixelsPerCell = 0;
        std::unique_ptr<SDL_Window> _window = nullptr;
        std::unique_ptr<SDL_Renderer> _renderer = nullptr;

        public:
        void openWindow(Vector2u pixelsWantedWindowSize);

        std::unique_ptr<RawTexture> loadTexture(const std::string &filename, char character, Color characterColor, Color backgroundColor, std::size_t width, std::size_t height);

        bool isButtonPressed(IDisplayModule::Button button);
        IDisplayModule::MouseButtonReleaseEvent getMouseButtonReleaseEvent();
        bool isClosing();

        void startTextInput();
        std::string getTextInput();
        void endTextInput();

        void update();

        void clearScreen(IDisplayModule::Color color);
        void renderSprite(IDisplayModule::Sprite sprite);
        void display();

        void setPixelsPerCell(std::uint32_t pixelsPerCell);
        std::uint32_t getPixelsPerCell();
    };

    const std::map<IDisplayModule::Color, SDL_Color> SDL_COLORS;
}