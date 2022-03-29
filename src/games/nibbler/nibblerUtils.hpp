/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** nibblerUtils
*/

#pragma once

#include "arcade-interface/ICore.hpp"
#include "textures.hpp"

namespace nibbler {
    enum Direction {
        UP, DOWN, LEFT, RIGHT,
    };

    enum TextureRotation {
        HORIZONTHAL,
        VERTICAL,
        UPLEFT,
        UPRIGHT,
        DOWNLEFT,
        DOWNRIGHT,
    };

    void movePos(ICore::Vector2u &pos, Direction dir, uint amount);
    Direction rotateDir(Direction dir, bool left);
    Direction inverseDir(Direction dir);
    TextureRotation getNibblerRotation(Direction actual, Direction next);
    ICore::Texture *getNibblerBodyTexture(TextureRotation rot, Textures &textures);
    ICore::Texture *getWallTexture(TextureRotation rot, Textures &textures);
}
