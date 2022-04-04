/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** nibblerGameModule
*/

#pragma once

#include <deque>
#include "arcade-interface/IGameModule.hpp"
#include "arena.hpp"
#include "textures.hpp"
#include "nibblerUtils.hpp"

class NibblerGameModule : public virtual IGameModule {
    using Vector2u = IDisplayModule::Vector2u;

    const Vector2u ARENA_SIZE = {16, 16};

    ICore *_coreHandle = nullptr;
    Textures _textures;

    std::deque<Vector2u> _snake;
    std::deque<nibbler::Direction> _snakeMove;
    nibbler::Direction _actualDir = nibbler::RIGHT;
    uint _lastMoveTick = 0;

    Arena _arena;

    uint _tick = 0;
    uint _score = 0;

    void reloadGame(const std::string &level);
    void loadLevel(const std::string &level);
    void readLevelChar(uint32_t x, uint32_t y, char c);


    void moveSnake();
    void checkCollision();
    bool growSnake();

    void drawSnake();
    void drawSnakeHead();
    void drawSnakeTail();

    public:
    void init(ICore *coreHandle) override;
    void update() override;
    void draw() override;
};
