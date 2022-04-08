/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** gamePhase
*/

#include "nibblerGameModule.hpp"
#include "gamePhase.hpp"

void nibbler::GamePhase::setup(NibblerGameModule *game)
{
    _lastMoveTick = 0;
    game->getArena().spawnFruit();
}

void nibbler::GamePhase::update(NibblerGameModule *game)
{
    ICore *core = game->getCore();
    ICore::Vector2u snakeHead;
    const auto &snakeMoves = game->getSnakeMoves();
    CollisionResult collision;

    if (core->isButtonPressed(ICore::Button::Left)
    && rotateDir(_actualDir, true) != snakeMoves.front())
        _actualDir = rotateDir(_actualDir, true);
    else if (core->isButtonPressed(ICore::Button::Right)
    && rotateDir(_actualDir, false) != snakeMoves.front())
        _actualDir = rotateDir(_actualDir, false);

    if (_lastMoveTick == 8) {
        game->moveSnake(_actualDir);
        collision = game->checkCollision();
        if (collision == WALL || collision == SNAKE)
            death(game);
        if (collision == FRUIT) {
            if (game->growSnake()) {
                snakeHead = game->getSnake().front();
                _score += game->getSnake().size() * 10;
                game->getArena().spawnFruit();
                game->getArena().setCellAt(snakeHead.x, snakeHead.y, Arena::Cell::EMPTY);
            } else
                death(game);
        }
        _lastMoveTick = 0;
    } else
        _lastMoveTick++;
}

void nibbler::GamePhase::death(NibblerGameModule *game)
{
    game->getCore()->addNewScore(_score);
    game->loadPhase(Phases::DEATH);
}

void nibbler::GamePhase::draw(NibblerGameModule *game)
{
    game->drawArena();
    game->drawSnake(-1, _lastMoveTick, 0);
    game->getScoreWriter().drawScore(_score, {0, 8 * 16}, 8);
}
