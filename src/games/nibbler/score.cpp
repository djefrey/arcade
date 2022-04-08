/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** score
*/

#include "score.hpp"

void nibbler::ScoreWriter::init(ICore *core)
{
    _coreHandle = core;
    for (int i = 0; i < 9; i++)
        _textures[i] = _coreHandle->loadTexture("./assets/font/emulogic.ttf", '0' + i, ICore::Color::white, ICore::Color::black, 8, 8);
}

void nibbler::ScoreWriter::drawScore(uint score, ICore::Vector2u start, uint digits)
{
    ICore::Vector2u pos = {start.x + (digits - 1) * 8, start.y};
    ICore::Sprite sprite;
    int digit;

    for (int i = digits - 1; i >= 0; i--) {
        digit = score % 10;
        sprite.pixelPosition = pos;
        sprite.texture = _textures[digit];
        _coreHandle->renderSprite(sprite);
        score /= 10;
        pos.x -= 8;
    }
}