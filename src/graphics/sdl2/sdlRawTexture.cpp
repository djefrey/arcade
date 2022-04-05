/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sdlRawTexture
*/

#include <stdexcept>
#include "sdlDisplay.hpp"
#include "sdlRawTexture.hpp"

sdl::SDLRawGraphicTexture::SDLRawGraphicTexture(const std::string &filename, SDL_Renderer *renderer)
{
    _texture = IMG_LoadTexture(renderer, filename.c_str());
    if (_texture == nullptr)
        throw std::runtime_error("Could not load texture " + filename);
}

SDL_Texture *sdl::SDLRawGraphicTexture::getTexture() const
{
    return _texture;
}

sdl::SDLRawGraphicTexture::~SDLRawGraphicTexture()
{
    SDL_DestroyTexture(_texture);
}

sdl::SDLRawASCIITexture::SDLRawASCIITexture(char c, Color textColor, Color bkgdColor, uint size, TTF_Font *font, SDL_Renderer *renderer)
{
    SDL_Color sdlTextColor = sdl::SDL_COLORS.at(textColor);
    SDL_Color sdlBkgdColor = sdl::SDL_COLORS.at(bkgdColor);

    TTF_SetFontSize(font, size);
    _surface = TTF_RenderGlyph32_Shaded(font, c, sdlTextColor, sdlBkgdColor);
    if (_texture == nullptr)
        throw std::runtime_error("Could not create surface '" + std::to_string(c) + "'");
    _texture = SDL_CreateTextureFromSurface(renderer, _surface);
    if (_texture == nullptr)
        throw std::runtime_error("Could not create texture '" + std::to_string(c) + "'");
}

SDL_Texture *sdl::SDLRawASCIITexture::getTexture() const
{
    return _texture;
}

sdl::SDLRawASCIITexture::~SDLRawASCIITexture()
{
    SDL_DestroyTexture(_texture);
    SDL_FreeSurface(_surface);
}
