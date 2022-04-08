/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** deathPhase
*/

#include "deathPhase.hpp"

void nibbler::DeathPhase::setup(NibblerGameModule *game)
{
    _snakeSize = game->getSnake().size();
}

void nibbler::DeathPhase::update(NibblerGameModule *game)
{
    if (_lastTickDestroy == 4) {
        if (_snakeSize == 0) {
            game->loadPhase(nibbler::Phases::INIT);
        } else {
            _snakeSize--;
            _lastTickDestroy = 0;
        }
    } else
        _lastTickDestroy++;
}

void nibbler::DeathPhase::draw(NibblerGameModule *game)
{
    game->drawSnake(_snakeSize, 0, 0);
}
