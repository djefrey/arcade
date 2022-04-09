/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** main
*/

#include <memory>
#include "oglDisplay.hpp"

std::unique_ptr<IDisplayModule> gEpitechArcadeGetDisplayModuleHandle()
{
    return std::make_unique<ogl::OpenGLDisplay>();
}
