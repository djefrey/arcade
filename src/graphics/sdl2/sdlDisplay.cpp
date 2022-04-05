/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sdlDisplay
*/

#include <stdexcept>
#include <filesystem>
#include "sdlDisplay.hpp"
#include "sdlRawTexture.hpp"

void sdl::SDLDisplay::openWindow(Vector2u size)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Could not init SDL");
    _window = std::unique_ptr<SDL_Window>(SDL_CreateWindow("Arcade", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, 0));
    if (_window.get() == nullptr)
        throw std::runtime_error("Could not create window");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    _renderer = std::unique_ptr<SDL_Renderer>(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED));
    if (_renderer.get() == nullptr)
        throw std::runtime_error("Could not create renderer");
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
        throw std::runtime_error("Could not init IMG module");
    if (TTF_Init() == -1)
        throw std::runtime_error("Could not init TTF module");
}

std::unique_ptr<IDisplayModule::RawTexture> sdl::SDLDisplay::loadTexture(const std::string &filename, char character, Color characterColor, Color backgroundColor, std::size_t width, std::size_t height)
{
    (void)height;

    std::filesystem::path filenamePath{filename};
    if (filenamePath.extension() == ".png")
        return std::make_unique<SDLRawGraphicTexture>(filename);
    if (filenamePath.extension() == ".ttf")
        return std::make_unique<SDLRawASCIITexture>(character, characterColor, backgroundColor, width, this->getFont(filename));
    throw std::runtime_error("Tried to load texture from invalid file !");
}

void sdl::SDLDisplay::clearScreen(Color color)
{
    SDL_Color sdlColor = SDL_COLORS.at(color);

    SDL_SetRenderDrawColor(_renderer.get(), sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
    SDL_RenderClear(_renderer.get());
}

void sdl::SDLDisplay::display()
{
    SDL_RenderPresent(_renderer.get());
}
