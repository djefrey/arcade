#pragma once

#include "arcade-interface/IDisplayModule.hpp"

namespace ncurses {
    class NCursesTexture : public IDisplayModule::RawTexture {
    public:
        virtual ~NCursesTexture() = default;

        NCursesTexture(char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height);
        char character;
        int ncursesColorPair;
        std::size_t width;
        std::size_t height;
    };
}
