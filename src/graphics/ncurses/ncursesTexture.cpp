#include "ncursesTexture.hpp"
#include "ncursesDisplay.hpp"
#include <assert.h>

ncurses::NCursesTexture::NCursesTexture(char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height)
    : character{character}, width{width}, height{height}
{
    auto ncursesForegroundColor = static_cast<std::size_t>(characterColor);
    auto ncursesBackgroundColor = static_cast<std::size_t>(backgroundColor);
    assert(ncursesForegroundColor < 8);
    assert(ncursesBackgroundColor < 8);

    this->ncursesColorPair = ncurses::getColorPair(characterColor, backgroundColor);
}
