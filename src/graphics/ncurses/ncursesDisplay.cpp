#include "ncursesDisplay.hpp"
#include "arcade-interface/IDisplayModule.hpp"
#include "ncursesTexture.hpp"
#include "utils/positionConverters.hpp"
#include <curses.h>
#include <iostream>
#include <stdexcept>

ncurses::NCursesDisplay::~NCursesDisplay()
{
    if (endwin() != OK)
        std::cerr << "endwin() failed (terminal not initialized ??\?)"; // The \ is to avoid triggering a trigraph
}

void ncurses::NCursesDisplay::openWindow(IDisplayModule::Vector2u pixelsWantedWindowSize)
{
    (void)pixelsWantedWindowSize;

    if (initscr() == nullptr)
        throw std::runtime_error("initscr() failed ???");
    if (cbreak() == ERR)
        throw std::runtime_error("cbreak() failed ???");
    if (keypad(stdscr, TRUE) == ERR)
        throw std::runtime_error("keypad(stdscr, TRUE) failed ???");
    if (nodelay(stdscr, TRUE) == ERR)
        throw std::runtime_error("nodelay(stdscr, TRUE) failed ???");
    if (noecho() == ERR)
        throw std::runtime_error("noecho() failed ???");
    if (intrflush(stdscr, TRUE) == ERR)
        throw std::runtime_error("intrflush(stdscr, TRUE) failed ???");
    if (!has_colors())
        throw std::runtime_error("has_colors() returned false... :(");
    if (start_color() == ERR)
        throw std::runtime_error("start_color() failed ???");
    if (curs_set(0) == ERR)
        throw std::runtime_error("curs_set(0) failed ???");

    static_assert(COLOR_BLACK >= 0 && COLOR_BLACK <= 7);
    static_assert(COLOR_RED >= 0 && COLOR_RED <= 7);
    static_assert(COLOR_GREEN >= 0 && COLOR_GREEN <= 7);
    static_assert(COLOR_YELLOW >= 0 && COLOR_YELLOW <= 7);
    static_assert(COLOR_BLUE >= 0 && COLOR_BLUE <= 7);
    static_assert(COLOR_MAGENTA >= 0 && COLOR_MAGENTA <= 7);
    static_assert(COLOR_CYAN >= 0 && COLOR_CYAN <= 7);
    static_assert(COLOR_WHITE >= 0 && COLOR_WHITE <= 7);
    std::size_t currentPair = 1;
    for (std::size_t foregroundColor = 0; foregroundColor <= 7; ++foregroundColor)
        for (std::size_t backgroundColor = 0; backgroundColor <= 7; ++backgroundColor)
            init_pair(currentPair++, foregroundColor, backgroundColor);

    mousemask(BUTTON1_RELEASED | BUTTON2_RELEASED, NULL);
}

std::unique_ptr<IDisplayModule::RawTexture> ncurses::NCursesDisplay::loadTexture(const std::string &filename, char character, IDisplayModule::Color characterColor, IDisplayModule::Color backgroundColor, std::size_t width, std::size_t height)
{
    (void)filename;

    return std::make_unique<NCursesTexture>(character, characterColor, backgroundColor, width, height);
}

void ncurses::NCursesDisplay::clearScreen(IDisplayModule::Color color)
{
    bkgd(COLOR_PAIR(ncurses::getColorPair(color, color)));
}

void ncurses::NCursesDisplay::renderSprite(IDisplayModule::Sprite sprite)
{
    NCursesTexture &texture = dynamic_cast<NCursesTexture &>(*sprite.texture);
    ICore::Vector2u spriteCenter = utils::posPixToCell(utils::getSquareCenter(sprite.rawPixelPosition, {static_cast<std::uint32_t>(texture.width), static_cast<std::uint32_t>(texture.height)}), this->pixelsPerCell);

    attron(COLOR_PAIR(texture.ncursesColorPair));
    mvaddch(spriteCenter.y, spriteCenter.x, texture.character);
    attroff(COLOR_PAIR(texture.ncursesColorPair));
}

void ncurses::NCursesDisplay::display()
{
    refresh();
    erase();
}

void ncurses::NCursesDisplay::update()
{
    this->buttonsPressedThisFrame.clear();
    this->textInputThisFrame.clear();
    this->mouseButtonReleaseEventThisFrame = {
        .type = IDisplayModule::MouseButtonReleaseEvent::Type::None,
        .cellPosition = {},
    };
 
    while (true) {
        int character = getch();
        if (character == ERR)
            break;
        auto displayButton = ncurses::NCURSES_KEY_TO_BUTTON.find(character);
        if (displayButton != ncurses::NCURSES_KEY_TO_BUTTON.end())
            this->buttonsPressedThisFrame[displayButton->second] = true;
        if (isprint(character))
            this->textInputThisFrame += character;
        if (character == KEY_MOUSE) {
            MEVENT mouseEvent;
            if (getmouse(&mouseEvent) != ERR) {
                if (mouseEvent.bstate & BUTTON1_RELEASED)
                    this->mouseButtonReleaseEventThisFrame.type = IDisplayModule::MouseButtonReleaseEvent::Type::Left;
                if (mouseEvent.bstate & BUTTON2_RELEASED)
                    this->mouseButtonReleaseEventThisFrame.type = IDisplayModule::MouseButtonReleaseEvent::Type::Right;
                this->mouseButtonReleaseEventThisFrame.cellPosition = {static_cast<std::uint32_t>(mouseEvent.x), static_cast<std::uint32_t>(mouseEvent.y)};
            }
        }
    }
}

bool ncurses::NCursesDisplay::isButtonPressed(IDisplayModule::Button button)
{
    auto it = this->buttonsPressedThisFrame.find(button);
    return it == this->buttonsPressedThisFrame.end() ? false : it->second;
}

IDisplayModule::MouseButtonReleaseEvent ncurses::NCursesDisplay::getMouseButtonReleaseEvent()
{
    return this->mouseButtonReleaseEventThisFrame;
}

bool ncurses::NCursesDisplay::isClosing()
{
    return false;
}

void ncurses::NCursesDisplay::startTextInput()
{
}

std::string ncurses::NCursesDisplay::getTextInput()
{
    return this->textInputThisFrame;
}

void ncurses::NCursesDisplay::endTextInput()
{
}
