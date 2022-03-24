#include "pacmanGameModule.hpp"

std::unique_ptr<IGameModule> gEpitechArcadeGetGameModuleHandle()
{
    return std::make_unique<PacmanGameModule>();
}
