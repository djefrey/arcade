/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** textures
*/

#pragma once

#include "arcade-interface/ICore.hpp"

class Textures {
    public:
    ICore::Texture *snakeHeadUp;
    ICore::Texture *snakeHeadDown;
    ICore::Texture *snakeHeadLeft;
    ICore::Texture *snakeHeadRight;

    ICore::Texture *snakeBodyHorizontal;
    ICore::Texture *snakeBodyVertical;
    ICore::Texture *snakeBodyCornerUpLeft;
    ICore::Texture *snakeBodyCornerUpRight;
    ICore::Texture *snakeBodyCornerDownLeft;
    ICore::Texture *snakeBodyCornerDownRight;

    ICore::Texture *snakeTailUp;
    ICore::Texture *snakeTailDown;
    ICore::Texture *snakeTailLeft;
    ICore::Texture *snakeTailRight;

    ICore::Texture *wallHorizontal;
    ICore::Texture *wallVertical;
    ICore::Texture *wallCornerUpLeft;
    ICore::Texture *wallCornerUpRight;
    ICore::Texture *wallCornerDownLeft;
    ICore::Texture *wallCornerDownRight;

    ICore::Texture *fruit;

    void init(ICore *coreHandler);
};