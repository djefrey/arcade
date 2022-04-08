#include <memory>
#include "ncursesDisplay.hpp"

std::unique_ptr<IDisplayModule> gEpitechArcadeGetDisplayModuleHandle()
{
    return std::make_unique<ncurses::NCursesDisplay>();
}

