/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** phase
*/

#pragma once

namespace nibbler {
    class NibblerGameModule;

    enum Phases {
        NONE,
        INIT,
        GAME,
        DEATH,
    };

    class Phase {
        public:
            virtual ~Phase() = default;

            virtual void setup(NibblerGameModule *game) = 0;
            virtual void update(NibblerGameModule *game) = 0;
            virtual void draw(NibblerGameModule *game) = 0;
    };

    std::unique_ptr<Phase> getPhase(Phases phase);
}
