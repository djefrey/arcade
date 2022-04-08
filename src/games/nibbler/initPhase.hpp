/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** initPhase
*/

#pragma once

#include "nibblerGameModule.hpp"
#include "phase.hpp"

namespace nibbler {
    class InitPhase : public Phase {
        const IDisplayModule::Vector2u ARENA_SIZE = {16, 16};

        uint _tick = 0;

        public:
        ~InitPhase() = default;

        void setup(NibblerGameModule *game);
        void update(NibblerGameModule *game);
        void draw(NibblerGameModule *game);
    };
};
