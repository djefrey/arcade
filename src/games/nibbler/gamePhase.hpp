/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** gamePhase
*/

#pragma once

#include "nibblerGameModule.hpp"
#include "phase.hpp"

namespace nibbler {
    class GamePhase : public Phase {
        Direction _actualDir = RIGHT;
        uint _lastMoveTick = 0;

        uint _score = 0;

        void death(NibblerGameModule *game);

        public:
        ~GamePhase() = default;

        void setup(NibblerGameModule *game);
        void update(NibblerGameModule *game);
        void draw(NibblerGameModule *game);
    };
}