/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** arena
*/

#pragma once

#include <memory>
#include <vector>
#include "nibblerUtils.hpp"

class Arena {
    using Vector2u = ICore::Vector2u;

    public:
    enum class Cell : uint8_t {
        EMPTY = 0,
        WALL = 1,
        FRUIT = 2,
    };

    void init(uint32_t width, uint32_t height);

    Cell getCellAt(uint32_t x, uint32_t y) const;
    void setCellAt(uint32_t x, uint32_t y, Cell cell);

    void addFruitSpawn(Vector2u pos);
    void spawnFruit();

    void drawArena(ICore *coreHandler, Textures &textures);

    nibbler::TextureRotation getWallRotation(uint32_t x, uint32_t y);

    private:
    uint32_t _width;
    uint32_t _height;

    std::unique_ptr<Cell[]> _cells;
    std::vector<Vector2u> _fruitSpawns;
};
