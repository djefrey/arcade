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
#include <unordered_map>
#include "arcade-interface/IDisplayModule.hpp"

namespace sdl {
    class SDLDisplay : public IDisplayModule {
        std::uint32_t _pixelsPerCell = 0;

        SDL_Window *_window = nullptr;
        SDL_Renderer *_renderer = nullptr;
        std::unordered_map<std::string, TTF_Font*> _fonts;

        bool _close = false;
        std::string _textInput;

        public:
        SDLDisplay();
        ~SDLDisplay();

        void openWindow(Vector2u pixelsWantedWindowSize);

        std::unique_ptr<RawTexture> loadTexture(const std::string &filename, char character, Color characterColor, Color backgroundColor, std::size_t width, std::size_t height);

        bool isButtonPressed(IDisplayModule::Button button);
        IDisplayModule::MouseButtonReleaseEvent getMouseButtonReleaseEvent();
        bool isClosing() { return _close; };

        void startTextInput();
        std::string getTextInput();
        void endTextInput();

        void update();

        void clearScreen(IDisplayModule::Color color);
        void renderSprite(IDisplayModule::Sprite sprite);
        void display();

        void setPixelsPerCell(std::uint32_t pixelsPerCell) { _pixelsPerCell = pixelsPerCell; };
        std::uint32_t getPixelsPerCell() { return _pixelsPerCell; };

        private:
        TTF_Font *getFont(const std::string &filepath);
    };

    extern const std::unordered_map<IDisplayModule::Color, SDL_Color> SDL_COLORS;
    extern const std::unordered_map<IDisplayModule::Button, SDL_Scancode> SDL_KEYS;
}