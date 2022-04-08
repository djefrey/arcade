/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** initPhase
*/

#include "initPhase.hpp"
#include "levels.hpp"

void nibbler::InitPhase::setup(NibblerGameModule *game)
{
    game->loadLevel(ARENA_SIZE, LEVEL_0);
}

void nibbler::InitPhase::update(NibblerGameModule *game)
{
    _tick++;
    if (_tick == 90)
        game->loadPhase(Phases::GAME);
}

void nibbler::InitPhase::draw(NibblerGameModule *game)
{
    if (_tick >= 30)
        game->drawArena();
    if (_tick >= 60)
        game->drawSnake(-1, 0, 0);
}
