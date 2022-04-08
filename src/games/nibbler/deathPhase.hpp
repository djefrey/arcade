/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** deathPhase
*/

#pragma once

#include "nibblerGameModule.hpp"
#include "phase.hpp"

namespace nibbler {
    class DeathPhase : public Phase {
        uint _lastTickDestroy = 0;
        uint _snakeSize = 0;

        public:
        ~DeathPhase() = default;

        void setup(NibblerGameModule *game);
        void update(NibblerGameModule *game);
        void draw(NibblerGameModule *game);
    };
};
