/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** nibblerGameModule
*/

#include <assert.h>
#include <iostream>
#include <algorithm>
#include "nibblerGameModule.hpp"
#include "utils/positionConverters.hpp"
#include "utils/vector2u.hpp"

void NibblerGameModule::init(ICore *coreHandle)
{
    _coreHandle = coreHandle;

    _coreHandle->setPixelsPerCell(8);
    _coreHandle->setFramerate(30);
    _coreHandle->openWindow({ARENA_SIZE.x * 8, ARENA_SIZE.y * 8});

    _textures.init(coreHandle);

    reloadGame();
}

void NibblerGameModule::reloadGame()
{
    _tick = 0;
    _lastMoveTick = 0;
    _score = 0;

    loadLevel();
}

void NibblerGameModule::loadLevel()
{
    constexpr Vector2u startingPos[4] = {
        {5, 5},
        {4, 5},
        {3, 5},
        {2, 5}
    };
    constexpr nibbler::Direction startingDir[4] = {
        nibbler::Direction::RIGHT,
        nibbler::Direction::RIGHT,
        nibbler::Direction::RIGHT,
        nibbler::Direction::RIGHT
    };

    _arena.init(ARENA_SIZE.x, ARENA_SIZE.y);
    _arena.addFruitSpawn({10, 10});
    _arena.addFruitSpawn({20, 16});
    _arena.addFruitSpawn({5, 22});
    _arena.addFruitSpawn({26, 14});

    _snake.clear();
    _snakeMove.clear();
    for (int i = 0; i < 4; i++) {
        _snake.push_back(startingPos[i]);
        _snakeMove.push_back(startingDir[i]);
    }
    _actualDir = nibbler::Direction::RIGHT;
}

void NibblerGameModule::update()
{
    if (_tick == 90)
        _arena.spawnFruit();
    if (_tick >= 90) {
        if (_coreHandle->isButtonPressed(ICore::Button::Left))
            _actualDir = nibbler::rotateDir(_actualDir, true);
        else if (_coreHandle->isButtonPressed(ICore::Button::Right))
            _actualDir = nibbler::rotateDir(_actualDir, false);

        moveSnake();
        checkCollision();

        _lastMoveTick++;
    }

    _tick++;
}

void NibblerGameModule::draw()
{
    _coreHandle->clearScreen(ICore::Color::black);
    if (_tick >= 30)
        _arena.drawArena(_coreHandle, _textures);
    if (_tick >= 60)
        drawSnake();
}

void NibblerGameModule::checkCollision()
{
    Vector2u head = _snake.front();
    Arena::Cell cell = _arena.getCellAt(head.x, head.y);

    if (cell == Arena::Cell::WALL || std::find(++_snake.begin(), _snake.end(), head) != _snake.end()) {
        reloadGame();
    } else if (cell == Arena::Cell::FRUIT) {
        if (growSnake()) {
            _arena.spawnFruit();
            _arena.setCellAt(head.x, head.y, Arena::Cell::EMPTY);
        } else
            reloadGame();
    }
}

bool NibblerGameModule::growSnake()
{
    Vector2u tail = _snake.back();
    nibbler::Direction dir = nibbler::inverseDir(_snakeMove.back());
    Vector2u pos = tail;

    for (int i = 0; i < 3; i++) {
        nibbler::movePos(pos, dir, 1);
        if (_arena.getCellAt(pos.x, pos.y) == Arena::Cell::EMPTY
        && std::find(_snake.begin(), _snake.end(), pos) == _snake.end()) {
            _snake.push_back(pos);
            _snakeMove.push_back(nibbler::inverseDir(dir));
            return true;
        }
        pos = tail;
        nibbler::rotateDir(dir, false);
        if (i == 1)
            nibbler::rotateDir(dir, false);
    }
    return false;
}

void NibblerGameModule::moveSnake()
{
    Vector2u head = _snake.front();

    if (_lastMoveTick == 8) {
        _snakeMove.pop_back();
        _snakeMove.push_front(_actualDir);

        assert(_snake.size() == _snakeMove.size());
        for (std::size_t i = 0; i < _snake.size(); i++)
            nibbler::movePos(_snake[i], _snakeMove[i], 1);

        _lastMoveTick = 0;
    }
}

void NibblerGameModule::drawSnake()
{
    ICore::Sprite sprite;
    nibbler::TextureRotation textureRot;

    assert(_snake.size() == _snakeMove.size());
    for (std::size_t i = 1; i < _snake.size() - 1; i++) {
        textureRot = nibbler::getNibblerRotation(_snakeMove[i], _snakeMove[i - 1]);
        sprite.pixelPosition = utils::posCellToPix(_snake[i], 8);
        sprite.texture = nibbler::getNibblerBodyTexture(textureRot, _textures);
        _coreHandle->renderSprite(sprite);
    }

    sprite.pixelPosition = utils::posCellToPix(_snake[1], 8);
    switch (_snakeMove.front()) {
        case nibbler::Direction::UP:
            sprite.texture = _textures.snakeHeadUp;
            break;
        case nibbler::Direction::DOWN:
            sprite.texture = _textures.snakeHeadDown;
            break;
        case nibbler::Direction::LEFT:
            sprite.texture = _textures.snakeHeadLeft;
            break;
        case nibbler::Direction::RIGHT:
            sprite.texture = _textures.snakeHeadRight;
            break;
    }
    nibbler::movePos(sprite.pixelPosition, _snakeMove.front(), 1 * _lastMoveTick);
    _coreHandle->renderSprite(sprite);

    sprite.pixelPosition = utils::posCellToPix(_snake.back(), 8);
    switch (_snakeMove.at(_snakeMove.size() - 2)) {
        case nibbler::Direction::UP:
            sprite.texture = _textures.snakeTailDown;
            break;
        case nibbler::Direction::DOWN:
            sprite.texture = _textures.snakeTailUp;
            break;
        case nibbler::Direction::LEFT:
            sprite.texture = _textures.snakeTailRight;
            break;
        case nibbler::Direction::RIGHT:
            sprite.texture = _textures.snakeTailLeft;
            break;
    }
    //nibbler::movePos(sprite.pixelPosition, _snakeMove.at(_snakeMove.size() - 2), 1 * _lastMoveTick);
    _coreHandle->renderSprite(sprite);
}
