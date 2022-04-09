/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** oglConstants
*/

#include "oglDisplay.hpp"

const std::unordered_map<IDisplayModule::Color, ogl::OGLColor> ogl::OPENGL_COLORS = {
    std::make_pair(IDisplayModule::Color::red,     OGLColor{255,   0,   0, 255}),
    std::make_pair(IDisplayModule::Color::green,   OGLColor{  0, 255,   0, 255}),
    std::make_pair(IDisplayModule::Color::blue,    OGLColor{  0,   0, 255, 255}),
    std::make_pair(IDisplayModule::Color::magenta, OGLColor{255,   0, 255, 255}),
    std::make_pair(IDisplayModule::Color::yellow,  OGLColor{255, 255,   0, 255}),
    std::make_pair(IDisplayModule::Color::cyan,    OGLColor{  0, 255, 255, 255}),
    std::make_pair(IDisplayModule::Color::black,   OGLColor{  0,   0,   0, 255}),
    std::make_pair(IDisplayModule::Color::white,   OGLColor{255, 255, 255, 255}),
};

const std::unordered_map<IDisplayModule::Color, sf::Color> ogl::SFML_COLORS = {
    std::make_pair(IDisplayModule::Color::red, sf::Color::Red),
    std::make_pair(IDisplayModule::Color::green, sf::Color::Green),
    std::make_pair(IDisplayModule::Color::blue, sf::Color::Blue),
    std::make_pair(IDisplayModule::Color::magenta, sf::Color::Magenta),
    std::make_pair(IDisplayModule::Color::yellow, sf::Color::Yellow),
    std::make_pair(IDisplayModule::Color::cyan, sf::Color::Cyan),
    std::make_pair(IDisplayModule::Color::black, sf::Color::Black),
    std::make_pair(IDisplayModule::Color::white, sf::Color::White),
};

const std::unordered_map<sf::Keyboard::Key, IDisplayModule::Button> ogl::SFML_BUTTONS = {
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

const std::string ogl::VERTEX_SHADER =
    "#version 330 core\n" \

    "layout(location = 0) in vec4 coords;\n" \

    "out vec2 UV;\n" \

    "void main() {\n" \
    "   gl_Position = vec4(coords.xy, 0.0, 1.0);\n" \
    "   UV = coords.zw;\n" \
    "}\n"
;

const std::string ogl::FRAGMENT_SHADER =
    "#version 330 core\n" \

    "in vec2 UV;\n" \

    "out vec4 color;\n" \
    "uniform sampler2D sampler;\n" \

    "void main() {\n" \
    "   color = texture(sampler, UV);\n" \
    "}\n"
;
