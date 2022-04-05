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

const std::unordered_map<IDisplayModule::Color, sf::Color> sfml::SFML_COLORS = {
    std::make_pair(IDisplayModule::Color::red, sf::Color::Red),
    std::make_pair(IDisplayModule::Color::green, sf::Color::Green),
    std::make_pair(IDisplayModule::Color::blue, sf::Color::Blue),
    std::make_pair(IDisplayModule::Color::magenta, sf::Color::Magenta),
    std::make_pair(IDisplayModule::Color::yellow, sf::Color::Yellow),
    std::make_pair(IDisplayModule::Color::cyan, sf::Color::Cyan),
    std::make_pair(IDisplayModule::Color::black, sf::Color::Black),
    std::make_pair(IDisplayModule::Color::white, sf::Color::White),
};

const std::unordered_map<sf::Keyboard::Key, IDisplayModule::Button> sfml::SFML_BUTTONS = {
    std::make_pair(sf::Keyboard::W, IDisplayModule::Button::Up),
    std::make_pair(sf::Keyboard::A, IDisplayModule::Button::Left),
    std::make_pair(sf::Keyboard::S, IDisplayModule::Button::Down),
    std::make_pair(sf::Keyboard::D, IDisplayModule::Button::Right),

    std::make_pair(sf::Keyboard::Left, IDisplayModule::Button::X),
    std::make_pair(sf::Keyboard::Up, IDisplayModule::Button::Y),
    std::make_pair(sf::Keyboard::Right, IDisplayModule::Button::B),
    std::make_pair(sf::Keyboard::Down, IDisplayModule::Button::A),

    std::make_pair(sf::Keyboard::Q, IDisplayModule::Button::L),
    std::make_pair(sf::Keyboard::E, IDisplayModule::Button::R),

    std::make_pair(sf::Keyboard::C, IDisplayModule::Button::Start),
    std::make_pair(sf::Keyboard::V, IDisplayModule::Button::Select),

    std::make_pair(sf::Keyboard::F1, IDisplayModule::Button::F1),
    std::make_pair(sf::Keyboard::F2, IDisplayModule::Button::F2),
    std::make_pair(sf::Keyboard::F3, IDisplayModule::Button::F3),
    std::make_pair(sf::Keyboard::F4, IDisplayModule::Button::F4),
    std::make_pair(sf::Keyboard::F5, IDisplayModule::Button::F5),
    std::make_pair(sf::Keyboard::F6, IDisplayModule::Button::F6),
    std::make_pair(sf::Keyboard::F7, IDisplayModule::Button::F7),
};
