/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sdlConstants
*/

#include "sdlDisplay.hpp"

const std::unordered_map<IDisplayModule::Color, SDL_Color> sdl::SDL_COLORS = {
    std::make_pair(IDisplayModule::Color::red,     SDL_Color{255,   0,   0, 255}),
    std::make_pair(IDisplayModule::Color::green,   SDL_Color{  0, 255,   0, 255}),
    std::make_pair(IDisplayModule::Color::blue,    SDL_Color{  0,   0, 255, 255}),
    std::make_pair(IDisplayModule::Color::magenta, SDL_Color{255,   0, 255, 255}),
    std::make_pair(IDisplayModule::Color::yellow,  SDL_Color{255, 255,   0, 255}),
    std::make_pair(IDisplayModule::Color::cyan,    SDL_Color{  0, 255, 255, 255}),
    std::make_pair(IDisplayModule::Color::black,   SDL_Color{  0,   0,   0, 255}),
    std::make_pair(IDisplayModule::Color::white,   SDL_Color{255, 255, 255, 255}),
};

const std::unordered_map<IDisplayModule::Button, SDL_Scancode> sdl::SDL_KEYS = {
    std::make_pair(IDisplayModule::Button::Up, SDL_SCANCODE_W),
    std::make_pair(IDisplayModule::Button::Left, SDL_SCANCODE_A),
    std::make_pair(IDisplayModule::Button::Down, SDL_SCANCODE_S),
    std::make_pair(IDisplayModule::Button::Right, SDL_SCANCODE_D),

    std::make_pair(IDisplayModule::Button::X, SDL_SCANCODE_LEFT),
    std::make_pair(IDisplayModule::Button::Y, SDL_SCANCODE_UP),
    std::make_pair(IDisplayModule::Button::B, SDL_SCANCODE_RIGHT),
    std::make_pair(IDisplayModule::Button::A, SDL_SCANCODE_DOWN),

    std::make_pair(IDisplayModule::Button::L, SDL_SCANCODE_Q),
    std::make_pair(IDisplayModule::Button::R, SDL_SCANCODE_E),

    std::make_pair(IDisplayModule::Button::Start, SDL_SCANCODE_C),
    std::make_pair(IDisplayModule::Button::Select, SDL_SCANCODE_V),

    std::make_pair(IDisplayModule::Button::F1, SDL_SCANCODE_F1),
    std::make_pair(IDisplayModule::Button::F2, SDL_SCANCODE_F2),
    std::make_pair(IDisplayModule::Button::F3, SDL_SCANCODE_F3),
    std::make_pair(IDisplayModule::Button::F4, SDL_SCANCODE_F4),
    std::make_pair(IDisplayModule::Button::F5, SDL_SCANCODE_F5),
    std::make_pair(IDisplayModule::Button::F6, SDL_SCANCODE_F6),
    std::make_pair(IDisplayModule::Button::F7, SDL_SCANCODE_F7),
};