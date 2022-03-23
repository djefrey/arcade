#include "menuGame.hpp"
#include "utils/positionConverters.hpp"
#include <limits>
#include <cassert>

void MenuGame::init(ICore *coreHandle)
{
    // Make sure we are actually passed the type we want
    this->coreHandle = &dynamic_cast<CoreImpl &>(*coreHandle);

    this->coreHandle->setPixelsPerCell(8);
    this->coreHandle->openWindow({640, 320});
    this->boxTopLeft = this->coreHandle->loadTexture("assets/menu/box-top-left.png", '/', ICore::Color::white, ICore::Color::black, 8, 8);
    this->boxTopRight = this->coreHandle->loadTexture("assets/menu/box-top-right.png", '\\', ICore::Color::white, ICore::Color::black, 8, 8);
    this->boxBottomLeft = this->coreHandle->loadTexture("assets/menu/box-bottom-left.png", '\\', ICore::Color::white, ICore::Color::black, 8, 8);
    this->boxBottomRight = this->coreHandle->loadTexture("assets/menu/box-bottom-right.png", '/', ICore::Color::white, ICore::Color::black, 8, 8);
    this->boxLeft = this->coreHandle->loadTexture("assets/menu/box-left.png", '|', ICore::Color::white, ICore::Color::black, 8, 8);
    this->boxRight = this->coreHandle->loadTexture("assets/menu/box-right.png", '|', ICore::Color::white, ICore::Color::black, 8, 8);
    this->boxTop = this->coreHandle->loadTexture("assets/menu/box-top.png", '-', ICore::Color::white, ICore::Color::black, 8, 8);
    this->boxBottom = this->coreHandle->loadTexture("assets/menu/box-bottom.png", '-', ICore::Color::white, ICore::Color::black, 8, 8);

    this->textHandler.init(coreHandle, 8, "assets/pacman-font/", true);
}

void MenuGame::update()
{
    if (this->coreHandle->isButtonPressed(ICore::Button::A) ||
        this->coreHandle->isButtonPressed(ICore::Button::Start)) {
        this->coreHandle->changeGameModule(this->coreHandle->getGameLibraries()[this->currentlySelectedGame].second());
        this->coreHandle->changeDisplayModule(this->coreHandle->getDisplayLibraries()[this->currentlySelectedDisplay].second());
        return;
    }
    if (this->coreHandle->isButtonPressed(ICore::Button::Left)) {
        if (this->currentlySelectedGame == 0)
            this->currentlySelectedGame = this->coreHandle->getGameLibraries().size() - 1;
        else
            --this->currentlySelectedGame;
    }
    if (this->coreHandle->isButtonPressed(ICore::Button::Right)) {
        ++this->currentlySelectedGame;
        this->currentlySelectedGame %= this->coreHandle->getGameLibraries().size();
    }
    if (this->coreHandle->isButtonPressed(ICore::Button::Up)) {
        if (this->currentlySelectedDisplay == 0)
            this->currentlySelectedDisplay = this->coreHandle->getDisplayLibraries().size() - 1;
        else
            --this->currentlySelectedDisplay;
    }
    if (this->coreHandle->isButtonPressed(ICore::Button::Down)) {
        ++this->currentlySelectedDisplay;
        this->currentlySelectedDisplay %= this->coreHandle->getDisplayLibraries().size();
    }
}

void MenuGame::draw()
{
    this->coreHandle->clearScreen(ICore::Color::black);
    this->textHandler.drawText("Arcade: Left/Right changes games, Up/Down changes display, a/start begins", std::numeric_limits<std::size_t>::max(), {0, 0});
    this->textHandler.drawText("D-Pad -> WASD, ABXY -> Arrow Keys, L & R -> Q and E, Start & Select -> C and V", std::numeric_limits<std::size_t>::max(), {0, 8});
    this->drawBox({0, 2}, {20, 38});
    this->drawBox({20, 2}, {20, 38});
    this->drawBox({40, 2}, {20, 38});
    this->drawBox({60, 2}, {20, 38});
    for (std::uint32_t i = 0; i < this->coreHandle->getGameLibraries().size() && i < 36; ++i) {
        this->textHandler.drawText(this->coreHandle->getGameLibraries()[i].first.getFileName(), 18, {2 * 8, 8 + (i * 8)});
        if (this->currentlySelectedGame == i)
            this->textHandler.drawText(">", 1, {1 * 8, 8 + (i * 8)});
    }
    for (std::uint32_t i = 0; i < this->coreHandle->getDisplayLibraries().size() && i < 36; ++i) {
        this->textHandler.drawText(this->coreHandle->getDisplayLibraries()[i].first.getFileName(), 18, {22 * 8, 8 + (i * 8)});
        if (this->currentlySelectedDisplay == i)
            this->textHandler.drawText(">", 1, {21 * 8, 8 + (i * 8)});
    }
}

void MenuGame::drawBox(ICore::Vector2u cellPosition, ICore::Vector2u boxSize)
{
    assert(boxSize.x > 1 && boxSize.y > 1);
    this->coreHandle->renderSprite({utils::cellPositionToRawPixelPosition(cellPosition, 8), this->boxTopLeft});
    this->coreHandle->renderSprite({utils::cellPositionToRawPixelPosition({cellPosition.x + boxSize.x - 1, cellPosition.y}, 8), this->boxTopRight});
    this->coreHandle->renderSprite({utils::cellPositionToRawPixelPosition({cellPosition.x, cellPosition.y + boxSize.y - 1}, 8), this->boxBottomLeft});
    this->coreHandle->renderSprite({utils::cellPositionToRawPixelPosition({cellPosition.x + boxSize.x - 1, cellPosition.y + boxSize.y - 1}, 8), this->boxBottomRight});
    for (std::uint32_t i = 1; i < (boxSize.x - 1); ++i) {
        this->coreHandle->renderSprite({utils::cellPositionToRawPixelPosition({cellPosition.x + i, cellPosition.y}, 8), this->boxTop});
        this->coreHandle->renderSprite({utils::cellPositionToRawPixelPosition({cellPosition.x + i, cellPosition.y + boxSize.y - 1}, 8), this->boxBottom});
    }
    for (std::uint32_t i = 1; i < (boxSize.y - 1); ++i) {
        this->coreHandle->renderSprite({utils::cellPositionToRawPixelPosition({cellPosition.x, cellPosition.y + i}, 8), this->boxLeft});
        this->coreHandle->renderSprite({utils::cellPositionToRawPixelPosition({cellPosition.x + boxSize.x - 1, cellPosition.y + i}, 8), this->boxRight});
    }
}
