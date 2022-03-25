/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sfmlConstants
*/

#include <map>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "sfmlDisplay.hpp"

const std::map<IDisplayModule::Color, sf::Color> sfml::SFML_COLORS = {
    std::make_pair(IDisplayModule::Color::red, sf::Color::Red),
    std::make_pair(IDisplayModule::Color::green, sf::Color::Green),
    std::make_pair(IDisplayModule::Color::blue, sf::Color::Blue),
    std::make_pair(IDisplayModule::Color::magenta, sf::Color::Magenta),
    std::make_pair(IDisplayModule::Color::yellow, sf::Color::Yellow),
    std::make_pair(IDisplayModule::Color::cyan, sf::Color::Cyan),
    std::make_pair(IDisplayModule::Color::black, sf::Color::Black),
    std::make_pair(IDisplayModule::Color::white, sf::Color::White),
    std::make_pair(IDisplayModule::Color::none, sf::Color::Transparent),
};

const std::map<IDisplayModule::Button, sf::Keyboard::Key> sfml::SFML_BUTTONS = {
    std::make_pair(IDisplayModule::Button::Up, sf::Keyboard::W),
    std::make_pair(IDisplayModule::Button::Left, sf::Keyboard::A),
    std::make_pair(IDisplayModule::Button::Down, sf::Keyboard::S),
    std::make_pair(IDisplayModule::Button::Right, sf::Keyboard::D),

    std::make_pair(IDisplayModule::Button::X, sf::Keyboard::Left),
    std::make_pair(IDisplayModule::Button::Y, sf::Keyboard::Up),
    std::make_pair(IDisplayModule::Button::B, sf::Keyboard::Right),
    std::make_pair(IDisplayModule::Button::A, sf::Keyboard::Down),

    std::make_pair(IDisplayModule::Button::L, sf::Keyboard::Q),
    std::make_pair(IDisplayModule::Button::R, sf::Keyboard::E),

    std::make_pair(IDisplayModule::Button::Start, sf::Keyboard::C),
    std::make_pair(IDisplayModule::Button::Select, sf::Keyboard::V),

    std::make_pair(IDisplayModule::Button::F1, sf::Keyboard::F1),
    std::make_pair(IDisplayModule::Button::F2, sf::Keyboard::F2),
    std::make_pair(IDisplayModule::Button::F3, sf::Keyboard::F3),
    std::make_pair(IDisplayModule::Button::F4, sf::Keyboard::F4),
    std::make_pair(IDisplayModule::Button::F5, sf::Keyboard::F5),
    std::make_pair(IDisplayModule::Button::F6, sf::Keyboard::F6),
    std::make_pair(IDisplayModule::Button::F7, sf::Keyboard::F7),
};
