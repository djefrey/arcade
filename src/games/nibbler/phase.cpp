/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** phase
*/

#include <memory>
#include "initPhase.hpp"
#include "gamePhase.hpp"
#include "deathPhase.hpp"

std::unique_ptr<nibbler::Phase> nibbler::getPhase(Phases phase)
{
    switch (phase) {
        case INIT:
        return std::make_unique<nibbler::InitPhase>();
        case GAME:
        return std::make_unique<nibbler::GamePhase>();
        case DEATH:
        return std::make_unique<nibbler::DeathPhase>();
    }
    return nullptr;
}