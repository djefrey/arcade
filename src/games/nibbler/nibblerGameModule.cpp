/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** NibblerGameModule
*/

#include <assert.h>
#include <iostream>
#include <algorithm>
#include "nibblerGameModule.hpp"
#include "utils/positionConverters.hpp"
#include "utils/vector2u.hpp"

void nibbler::NibblerGameModule::init(ICore *coreHandle)
{
    _coreHandle = coreHandle;

    _coreHandle->setPixelsPerCell(8);
    _coreHandle->setFramerate(30);
    _coreHandle->openWindow({ARENA_SIZE.x * 8, (ARENA_SIZE.y + 1) * 8});

    _textures.init(coreHandle);
    _scoreWriter.init(coreHandle);
}

void nibbler::NibblerGameModule::loadPhase(nibbler::Phases phase)
{
    _nextPhase = phase;
}

void nibbler::NibblerGameModule::loadLevel(Vector2u size, const std::string &level)
{
    const Vector2u startingPos[4] = {
        {10, 8}, {9, 8}, {8, 8}, {7, 8},
    };
    const Direction startingDir[4] = {
        RIGHT, RIGHT, RIGHT, RIGHT,
    };

    _snake.clear();
    _snakeMove.clear();
    _arena.init(size.x, size.y);
    for (int i = 0; i < 4; i++) {
        _snake.push_back(startingPos[i]);
        _snakeMove.push_back(startingDir[i]);
    }
    for (uint32_t y = 0; y < size.y; y++) {
        for (uint32_t x = 0; x < size.x; x++) {
            readLevelChar(x, y, level[x + y * size.x]);
        }
    }
}

void nibbler::NibblerGameModule::readLevelChar(uint32_t x, uint32_t y, char c)
{
    if (c == 'X') {
        _arena.setCellAt(x, y, Arena::Cell::WALL);
        return;
    }
    _arena.setCellAt(x, y, Arena::Cell::EMPTY);
    if (c == 'o')
        _arena.addFruitSpawn({x, y});
}

void nibbler::NibblerGameModule::update()
{
    if (_nextPhase != Phases::NONE) {
        _actualPhase = getPhase(_nextPhase);
        _nextPhase = Phases::NONE;
        _actualPhase->setup(this);
    }
    _actualPhase->update(this);
}

void nibbler::NibblerGameModule::draw()
{
    _coreHandle->clearScreen(ICore::Color::black);
    _actualPhase->draw(this);
}

nibbler::CollisionResult nibbler::NibblerGameModule::checkCollision()
{
    Vector2u head = _snake.front();
    Arena::Cell cell = _arena.getCellAt(head.x, head.y);

    if (std::find(++_snake.begin(), _snake.end(), head) != _snake.end())
        return SNAKE;
    else if (cell == Arena::Cell::WALL)
        return WALL;
    else if (cell == Arena::Cell::FRUIT)
        return FRUIT;
    return NOTHING;
}

bool nibbler::NibblerGameModule::growSnake()
{
    Vector2u tail = _snake.back();
    Direction dir = inverseDir(_snakeMove.back());
    Vector2u pos = tail;

    for (int i = 0; i < 3; i++) {
        movePos(pos, dir, 1);
        if (_arena.getCellAt(pos.x, pos.y) == Arena::Cell::EMPTY
        && std::find(_snake.begin(), _snake.end(), pos) == _snake.end()) {
            _snake.push_back(pos);
            _snakeMove.push_back(inverseDir(dir));
            return true;
        }
        pos = tail;
        rotateDir(dir, false);
        if (i == 1)
            rotateDir(dir, false);
    }
    return false;
}

void nibbler::NibblerGameModule::moveSnake(nibbler::Direction dir)
{
    Vector2u head = _snake.front();

    _snakeMove.pop_back();
    _snakeMove.push_front(dir);

    for (std::size_t i = 0; i < _snake.size(); i++)
        movePos(_snake[i], _snakeMove[i], 1);
}

void nibbler::NibblerGameModule::drawArena()
{
    _arena.drawArena(_coreHandle, _textures);
}

void nibbler::NibblerGameModule::drawSnake(int nbCells, uint headOffset, uint tailOffset)
{
    ICore::Sprite sprite;
    TextureRotation textureRot;

    if (nbCells == 0)
        return;

    for (std::size_t i = 1; i < _snake.size() - 1; i++) {
        if (nbCells == i)
            break;
        textureRot = getNibblerRotation(_snakeMove[i], _snakeMove[i - 1]);
        sprite.pixelPosition = utils::posCellToPix(_snake[i], 8);
        sprite.texture = getNibblerBodyTexture(textureRot, _textures);
        _coreHandle->renderSprite(sprite);
    }

    if (nbCells != 0)
        drawSnakeHead(headOffset);
    if (nbCells == -1 || nbCells == _snake.size())
        drawSnakeTail(tailOffset);
}

void nibbler::NibblerGameModule::drawSnakeHead(uint offset)
{
    ICore::Sprite sprite;
    TextureRotation textureRot;

    sprite.pixelPosition = utils::posCellToPix(_snake[1], 8);
    switch (_snakeMove.front()) {
        case Direction::UP:
            sprite.texture = _textures.snakeHeadUp;
            break;
        case Direction::DOWN:
            sprite.texture = _textures.snakeHeadDown;
            break;
        case Direction::LEFT:
            sprite.texture = _textures.snakeHeadLeft;
            break;
        case Direction::RIGHT:
            sprite.texture = _textures.snakeHeadRight;
            break;
    }
    movePos(sprite.pixelPosition, _snakeMove.front(), offset);
    _coreHandle->renderSprite(sprite);
}

void nibbler::NibblerGameModule::drawSnakeTail(uint offset)
{
    ICore::Sprite sprite;
    TextureRotation textureRot;

    sprite.pixelPosition = utils::posCellToPix(_snake.back(), 8);
    switch (_snakeMove.at(_snakeMove.size() - 2)) {
        case Direction::UP:
            sprite.texture = _textures.snakeTailDown;
            break;
        case Direction::DOWN:
            sprite.texture = _textures.snakeTailUp;
            break;
        case Direction::LEFT:
            sprite.texture = _textures.snakeTailRight;
            break;
        case Direction::RIGHT:
            sprite.texture = _textures.snakeTailLeft;
            break;
    }
    movePos(sprite.pixelPosition, _snakeMove.at(_snakeMove.size() - 2), offset);
    _coreHandle->renderSprite(sprite);
}
