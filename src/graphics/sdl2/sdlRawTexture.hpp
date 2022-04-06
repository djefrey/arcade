/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sdlRawTexture
*/

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "arcade-interface/IDisplayModule.hpp"

namespace sdl {
    class ASDLRawTexture : public IDisplayModule::RawTexture {
        public:
        virtual ~ASDLRawTexture() = default;
        virtual SDL_Texture *getTexture() const = 0;
        virtual IDisplayModule::Vector2u getSize() const = 0;
    };

    class SDLRawGraphicTexture : public ASDLRawTexture {
        SDL_Texture *_texture;
        IDisplayModule::Vector2u _size;

        public:
        SDLRawGraphicTexture(const std::string &filename, IDisplayModule::Vector2u size, SDL_Renderer *renderer);
        ~SDLRawGraphicTexture();

        SDL_Texture *getTexture() const { return _texture; };
        IDisplayModule::Vector2u getSize() const { return _size; };
    };

    class SDLRawASCIITexture : public ASDLRawTexture {
        using Color = IDisplayModule::Color;

        SDL_Surface *_surface;
        SDL_Texture *_texture;
        uint _size;

        public:
        SDLRawASCIITexture(char c, Color textColor, Color bkgdColor, uint size, TTF_Font *font, SDL_Renderer *renderer);
        ~SDLRawASCIITexture();

        SDL_Texture *getTexture() const { return _texture; };
        IDisplayModule::Vector2u getSize() const { return {_size, _size}; };
    };
}