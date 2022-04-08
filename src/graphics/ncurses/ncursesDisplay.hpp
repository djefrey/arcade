#pragma once

#include "arcade-interface/IDisplayModule.hpp"
#include <unordered_map>
#include <curses.h>
#include <assert.h>

namespace ncurses {
    class NCursesDisplay : public IDisplayModule {
        std::unordered_map<IDisplayModule::Button, bool> buttonsPressedThisFrame;
        std::string textInputThisFrame;
        IDisplayModule::MouseButtonReleaseEvent mouseButtonReleaseEventThisFrame;

    public:
        ~NCursesDisplay();

        std::unique_ptr<IDisplayModule::RawTexture> loadTexture(const std::string &filename, char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height) override;

        void openWindow(IDisplayModule::Vector2u pixelsWantedWindowSize) override;

        bool isButtonPressed(IDisplayModule::Button button) override;
        IDisplayModule::MouseButtonReleaseEvent getMouseButtonReleaseEvent() override;
        bool isClosing() override;

        void startTextInput() override;
        std::string getTextInput() override;
        void endTextInput() override;

        void clearScreen(IDisplayModule::Color color) override;
        void renderSprite(IDisplayModule::Sprite sprite) override;
        void display() override;
        void update() override;

        std::size_t pixelsPerCell;
        void setPixelsPerCell(std::uint32_t pixelsPerCell) override
        {
            this->pixelsPerCell = pixelsPerCell;
        }
        std::uint32_t getPixelsPerCell() override
        {
            return this->pixelsPerCell;
        }
    };

    inline int getColorPair(IDisplayModule::Color foregroundColor, IDisplayModule::Color backgroundColor)
    {
        auto szForegroundColor = static_cast<std::size_t>(foregroundColor);
        auto szBackgroundColor = static_cast<std::size_t>(backgroundColor);
        assert(szForegroundColor < 8);
        assert(szBackgroundColor < 8);

        return 1 + (szForegroundColor * 8) + szBackgroundColor;
    }

    inline std::unordered_map<int, IDisplayModule::Button> NCURSES_KEY_TO_BUTTON = {
        {'a', IDisplayModule::Button::Left},
        {'d', IDisplayModule::Button::Right},
        {'w', IDisplayModule::Button::Up},
        {'s', IDisplayModule::Button::Down},
        {KEY_DOWN, IDisplayModule::Button::A},
        {KEY_RIGHT, IDisplayModule::Button::B},
        {KEY_LEFT, IDisplayModule::Button::X},
        {KEY_UP, IDisplayModule::Button::Y},
        {'q', IDisplayModule::Button::L},
        {'e', IDisplayModule::Button::R},
        {'c', IDisplayModule::Button::Start},
        {'v', IDisplayModule::Button::Select},
        {KEY_F(1), IDisplayModule::Button::F1},
        {KEY_F(2), IDisplayModule::Button::F2},
        {KEY_F(3), IDisplayModule::Button::F3},
        {KEY_F(4), IDisplayModule::Button::F4},
        {KEY_F(5), IDisplayModule::Button::F5},
        {KEY_F(6), IDisplayModule::Button::F6},
        {KEY_F(7), IDisplayModule::Button::F7},
    };
}
