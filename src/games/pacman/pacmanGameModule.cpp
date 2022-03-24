#include "pacmanGameModule.hpp"

#include <stdio.h>

void PacmanGameModule::init(ICore *coreHandle)
{
    this->coreHandle = coreHandle;
}

void PacmanGameModule::update()
{
    static unsigned bruh = 0;

    if (!((++bruh) % 60))
        puts("one second");
}

void PacmanGameModule::draw()
{
    this->coreHandle->clearScreen(ICore::Color::red);
}
