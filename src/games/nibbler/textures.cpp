/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** textures
*/

#include "textures.hpp"

void Textures::init(ICore *coreHandler)
{
    this->snakeHeadUp = coreHandler->loadTexture("./assets/nibbler/snake-head-up.png", '^', ICore::Color::white, ICore::Color::red, 8, 8);
    this->snakeHeadDown = coreHandler->loadTexture("./assets/nibbler/snake-head-down.png", 'V', ICore::Color::white, ICore::Color::red, 8, 8);
    this->snakeHeadLeft = coreHandler->loadTexture("./assets/nibbler/snake-head-left.png", '<', ICore::Color::white, ICore::Color::red, 8, 8);
    this->snakeHeadRight = coreHandler->loadTexture("./assets/nibbler/snake-head-right.png", '>', ICore::Color::white, ICore::Color::red, 8, 8);

    this->snakeBodyHorizontal = coreHandler->loadTexture("./assets/nibbler/snake-body-horizonthal.png", '-', ICore::Color::white, ICore::Color::black, 8, 8);
    this->snakeBodyVertical = coreHandler->loadTexture("./assets/nibbler/snake-body-vertical.png", '|', ICore::Color::white, ICore::Color::black, 8, 8);
    this->snakeBodyCornerUpLeft = coreHandler->loadTexture("./assets/nibbler/snake-body-upleft.png", '/', ICore::Color::white, ICore::Color::black, 8, 8);
    this->snakeBodyCornerUpRight = coreHandler->loadTexture("./assets/nibbler/snake-body-upright.png", '\\', ICore::Color::white, ICore::Color::black, 8, 8);
    this->snakeBodyCornerDownLeft = coreHandler->loadTexture("./assets/nibbler/snake-body-downleft.png", '\\', ICore::Color::white, ICore::Color::black, 8, 8);
    this->snakeBodyCornerDownRight = coreHandler->loadTexture("./assets/nibbler/snake-body-downright.png", '/', ICore::Color::white, ICore::Color::black, 8, 8);

    this->snakeTailUp = coreHandler->loadTexture("./assets/nibbler/snake-tail-up.png", ',', ICore::Color::white, ICore::Color::red, 8, 8);
    this->snakeTailDown = coreHandler->loadTexture("./assets/nibbler/snake-tail-down.png", '\'', ICore::Color::white, ICore::Color::red, 8, 8);
    this->snakeTailLeft = coreHandler->loadTexture("./assets/nibbler/snake-tail-left.png", '~', ICore::Color::white, ICore::Color::red, 8, 8);
    this->snakeTailRight = coreHandler->loadTexture("./assets/nibbler/snake-tail-right.png", '~', ICore::Color::white, ICore::Color::red, 8, 8);

    this->wallHorizontal = coreHandler->loadTexture("./assets/nibbler/wall-horizonthal.png", '-', ICore::Color::white, ICore::Color::black, 8, 8);
    this->wallVertical = coreHandler->loadTexture("./assets/nibbler/wall-vertical.png", '|', ICore::Color::white, ICore::Color::black, 8, 8);
    this->wallCornerUpLeft = coreHandler->loadTexture("./assets/nibbler/wall-corner-upleft.png", '/', ICore::Color::white, ICore::Color::black, 8, 8);
    this->wallCornerUpRight = coreHandler->loadTexture("./assets/nibbler/wall-corner-upright.png", '\\', ICore::Color::white, ICore::Color::black, 8, 8);
    this->wallCornerDownLeft = coreHandler->loadTexture("./assets/nibbler/wall-corner-downleft.png", '\\', ICore::Color::white, ICore::Color::black, 8, 8);
    this->wallCornerDownRight = coreHandler->loadTexture("./assets/nibbler/wall-corner-downright.png", '/', ICore::Color::white, ICore::Color::black, 8, 8);

    this->fruit = coreHandler->loadTexture("./assets/nibbler/fruit.png", '#', ICore::Color::yellow, ICore::Color::black, 8, 8);
}