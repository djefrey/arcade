/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** score
*/

#pragma once

#include "arcade-interface/ICore.hpp"

namespace nibbler {
    class ScoreWriter {
        ICore *_coreHandle;
        ICore::Texture* _textures[9] = {nullptr};

        public:
        void init(ICore *core);
        void drawScore(uint score, ICore::Vector2u start, uint digits);
    };
}