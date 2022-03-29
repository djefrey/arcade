/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** arena
*/

#include <assert.h>
#include <algorithm>
#include <memory>
#include "arena.hpp"
#include "utils/positionConverters.hpp"

void Arena::init(uint32_t width, uint32_t height)
{
    _width = width;
    _height = height;
    _cells = std::unique_ptr<Cell>(new Cell[width * height]);
    _fruitSpawns.clear();

    for (std::size_t i = 0; i < width * height; i++)
        _cells.get()[i] = Cell::EMPTY;

    for (uint32_t x = 0; x < width; x++) {
        setCellAt(x,          0, Cell::WALL);
        setCellAt(x, height - 1, Cell::WALL);
    }
    for (uint32_t y = 0; y < height; y++) {
        setCellAt(0,         y, Cell::WALL);
        setCellAt(width - 1, y, Cell::WALL);
    }
}

Arena::Cell Arena::getCellAt(uint32_t x, uint32_t y) const
{
    if (x >= _width || y >= _height)
        return Cell::EMPTY;
    return _cells.get()[x + y * _width];
}

void Arena::setCellAt(uint32_t x, uint32_t y, Cell cell)
{
    if (x < _width && y < _height)
        _cells.get()[x + y * _width] = cell;
}

void Arena::addFruitSpawn(Vector2u pos)
{
    for (Vector2u spawnPos : _fruitSpawns) {
        if (spawnPos.x == pos.x && spawnPos.y == pos.y)
            return;
    }
    _fruitSpawns.push_back(pos);
}

void Arena::spawnFruit()
{
    std::vector<Vector2u> emptyCells;
    Vector2u pos;

    for (Vector2u spawnPos : _fruitSpawns) {
        if (getCellAt(spawnPos.x, spawnPos.y) != Cell::EMPTY)
            continue;
        emptyCells.push_back(spawnPos);
    }
    pos = emptyCells.at(std::rand() % emptyCells.size());
    setCellAt(pos.x, pos.y, Cell::FRUIT);
}

void Arena::drawArena(ICore *coreHandler, Textures &textures)
{
    ICore::Sprite sprite;
    Cell cell;

    for (uint32_t y = 0; y < _height; y++) {
        for (uint32_t x = 0; x < _width; x++) {
            cell = getCellAt(x, y);
            if (cell == Cell::EMPTY)
                continue;
            sprite.pixelPosition = utils::posCellToPix({x, y}, 8);
            if (cell == Cell::FRUIT)
                sprite.texture = textures.fruit;
            else
                sprite.texture = nibbler::getWallTexture(getWallRotation(x, y), textures);
            coreHandler->renderSprite(sprite);
        }
    }
}

nibbler::TextureRotation Arena::getWallRotation(uint32_t x, uint32_t y)
{
    Cell up = getCellAt(x, y - 1);
    Cell down = getCellAt(x, y + 1);
    Cell left = getCellAt(x - 1, y);
    Cell right = getCellAt(x + 1, y);

    assert(getCellAt(x, y) == Cell::WALL);
    if (up == Cell::WALL && down != Cell::WALL) {
        if (left == Cell::WALL)
            return nibbler::TextureRotation::UPLEFT;
        else if (right == Cell::WALL)
            return nibbler::TextureRotation::UPRIGHT;
        else
            return nibbler::TextureRotation::VERTICAL;
    } else if (up != Cell::WALL && down == Cell::WALL) {
        if (left == Cell::WALL)
            return nibbler::TextureRotation::DOWNLEFT;
        else if (right == Cell::WALL)
            return nibbler::TextureRotation::DOWNRIGHT;
        else
            return nibbler::TextureRotation::VERTICAL;
    } else if (up == Cell::WALL) {
        return nibbler::TextureRotation::VERTICAL;
    } else {
        return nibbler::TextureRotation::HORIZONTHAL;
    }
}