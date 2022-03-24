/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** sfmlConstants
*/

#include <map>
#include <SFML/Graphics/Color.hpp>
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
};
