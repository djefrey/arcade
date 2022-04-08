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
#include "levels.hpp"

void NibblerGameModule::init(ICore *coreHandle)
{
    _coreHandle = coreHandle;

    _coreHandle->setPixelsPerCell(8);
    _coreHandle->setFramerate(30);
    _coreHandle->openWindow({ARENA_SIZE.x * 8, ARENA_SIZE.y * 8});

    _textures.init(coreHandle);

    reloadGame(LEVEL_0);
}

void NibblerGameModule::reloadGame(const std::string &level)
{
    _tick = 0;
    _lastMoveTick = 0;
    _score = 0;

    loadLevel(level);
}

void NibblerGameModule::loadLevel(const std::string &level)
{
    const Vector2u startingPos[4] = {
        {10, 8}, {9, 8}, {8, 8}, {7, 8},
    };
    const nibbler::Direction startingDir[4] = {
        nibbler::RIGHT, nibbler::RIGHT, nibbler::RIGHT, nibbler::RIGHT,
    };

    _snake.clear();
    _snakeMove.clear();
    _arena.init(ARENA_SIZE.x, ARENA_SIZE.y);
    for (int i = 0; i < 4; i++) {
        _snake.push_back(startingPos[i]);
        _snakeMove.push_back(startingDir[i]);
    }
    for (uint32_t y = 0; y < ARENA_SIZE.y; y++) {
        for (uint32_t x = 0; x < ARENA_SIZE.x; x++) {
            readLevelChar(x, y, level[x + y * ARENA_SIZE.x]);
        }
    }
    _actualDir = startingDir[0];
}

void NibblerGameModule::readLevelChar(uint32_t x, uint32_t y, char c)
{
    if (c == 'X') {
        _arena.setCellAt(x, y, Arena::Cell::WALL);
        return;
    }
    _arena.setCellAt(x, y, Arena::Cell::EMPTY);
    if (c == 'o')
        _arena.addFruitSpawn({x, y});
}

void NibblerGameModule::update()
{
    if (_tick == 90)
        _arena.spawnFruit();
    if (_tick >= 90) {
        if (_coreHandle->isButtonPressed(ICore::Button::Left)
        && nibbler::rotateDir(_actualDir, true) != _snakeMove.front())
            _actualDir = nibbler::rotateDir(_actualDir, true);
        else if (_coreHandle->isButtonPressed(ICore::Button::Right)
        && nibbler::rotateDir(_actualDir, false) != _snakeMove.front())
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
        reloadGame(LEVEL_0);
    } else if (cell == Arena::Cell::FRUIT) {
        if (growSnake()) {
            _arena.spawnFruit();
            _arena.setCellAt(head.x, head.y, Arena::Cell::EMPTY);
        } else
            reloadGame(LEVEL_0);
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
