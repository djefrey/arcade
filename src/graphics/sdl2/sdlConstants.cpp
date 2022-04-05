/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sdlConstants
*/

#include "sdlDisplay.hpp"

const std::map<IDisplayModule::Color, SDL_Color> sdl::SDL_COLORS = {
    std::make_pair(IDisplayModule::Color::red,     SDL_Color{255,   0,   0, 255}),
    std::make_pair(IDisplayModule::Color::green,   SDL_Color{  0, 255,   0, 255}),
    std::make_pair(IDisplayModule::Color::blue,    SDL_Color{  0,   0, 255, 255}),
    std::make_pair(IDisplayModule::Color::magenta, SDL_Color{255,   0, 255, 255}),
    std::make_pair(IDisplayModule::Color::yellow,  SDL_Color{255, 255,   0, 255}),
    std::make_pair(IDisplayModule::Color::cyan,    SDL_Color{  0, 255, 255, 255}),
    std::make_pair(IDisplayModule::Color::black,   SDL_Color{  0,   0,   0, 255}),
    std::make_pair(IDisplayModule::Color::white,   SDL_Color{255, 255, 255, 255}),
    std::make_pair(IDisplayModule::Color::none,    SDL_Color{  0,   0,  0,   0}),
};
