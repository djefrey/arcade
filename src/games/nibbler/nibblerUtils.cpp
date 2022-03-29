/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** nibblerUtils
*/

#include "nibblerGameModule.hpp"
#include "utils/positionConverters.hpp"

void nibbler::movePos(ICore::Vector2u &pos, Direction dir, uint amount)
{
    switch (dir) {
        case UP:
            pos.y -= amount;
            return;
        case DOWN:
            pos.y += amount;
            return;
        case LEFT:
            pos.x -= amount;
            return;
        case RIGHT:
            pos.x += amount;
            return;
    }
}

nibbler::Direction nibbler::rotateDir(Direction dir, bool left)
{
    switch (dir) {
        case UP:
            return left ? LEFT : RIGHT;
        case DOWN:
            return left ? RIGHT : LEFT;
        case LEFT:
            return left ? DOWN : UP;
        case RIGHT:
            return left ? UP : DOWN;
    }
    return UP;
}

nibbler::Direction nibbler::inverseDir(Direction dir)
{
    switch (dir) {
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case LEFT:
            return RIGHT;
        case RIGHT:
            return LEFT;
    }
    return UP;
}

nibbler::TextureRotation nibbler::getNibblerRotation(Direction actual, Direction next)
{
    if (actual == next) {
        return actual == LEFT || actual == RIGHT ? HORIZONTHAL : VERTICAL;
    } else {
        switch (actual) {
            case UP:
                return next == LEFT ? DOWNLEFT : DOWNRIGHT;
            case DOWN:
                return next == LEFT ? UPLEFT : UPRIGHT;
            case LEFT:
                return next == UP ? UPRIGHT : DOWNRIGHT;
            case RIGHT:
                return next == UP ? UPLEFT : DOWNLEFT;
        }
        return VERTICAL;
    }
}

ICore::Texture *nibbler::getNibblerBodyTexture(nibbler::TextureRotation rot, Textures &textures)
{
    switch (rot) {
        case HORIZONTHAL:
            return textures.snakeBodyHorizontal;
        case VERTICAL:
            return textures.snakeBodyVertical;
        case UPLEFT:
            return textures.snakeBodyCornerUpLeft;
        case UPRIGHT:
            return textures.snakeBodyCornerUpRight;
        case DOWNLEFT:
            return textures.snakeBodyCornerDownLeft;
        case DOWNRIGHT:
            return textures.snakeBodyCornerDownRight;
    }
    return textures.snakeBodyVertical;
}

ICore::Texture *nibbler::getWallTexture(nibbler::TextureRotation rot, Textures &textures)
{
    switch (rot) {
        case HORIZONTHAL:
            return textures.wallHorizontal;
        case VERTICAL:
            return textures.wallVertical;
        case UPLEFT:
            return textures.wallCornerUpLeft;
        case UPRIGHT:
            return textures.wallCornerUpRight;
        case DOWNLEFT:
            return textures.wallCornerDownLeft;
        case DOWNRIGHT:
            return textures.wallCornerDownRight;
    }
    return textures.wallVertical;
}

