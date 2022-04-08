/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** nibblerGameModule
*/

#pragma once

#include <deque>
#include "arcade-interface/IGameModule.hpp"
#include "phase.hpp"
#include "arena.hpp"
#include "textures.hpp"
#include "score.hpp"
#include "nibblerUtils.hpp"

namespace nibbler {
    class NibblerGameModule : public virtual IGameModule {
        using Vector2u = IDisplayModule::Vector2u;

        const Vector2u ARENA_SIZE = {16, 16};

        ICore *_coreHandle = nullptr;
        Textures _textures;

        std::unique_ptr<Phase> _actualPhase = nullptr;
        Phases _nextPhase = Phases::INIT;

        Arena _arena;
        ScoreWriter _scoreWriter;

        std::deque<Vector2u> _snake;
        std::deque<Direction> _snakeMove;

        void readLevelChar(uint32_t x, uint32_t y, char c);

        void drawSnakeHead(uint offset);
        void drawSnakeTail(uint offset);

        public:
        void init(ICore *coreHandle) override;
        void update() override;
        void draw() override;

        ICore *getCore() { return _coreHandle; };
        Arena &getArena() { return _arena; };
        ScoreWriter &getScoreWriter() { return _scoreWriter; };
        const std::deque<Vector2u> getSnake() { return _snake; };
        const std::deque<Direction> getSnakeMoves() { return _snakeMove; };

        void loadPhase(Phases phase);
        void loadLevel(Vector2u size, const std::string &level);

        void drawArena();

        void drawSnake(int nbCells, uint headOffset, uint tailOffset);
        void moveSnake(Direction dir);

        CollisionResult checkCollision();
        bool growSnake();
    };
}