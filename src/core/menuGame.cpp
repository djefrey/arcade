#include "menuGame.hpp"
#include "utils/positionConverters.hpp"
#include <algorithm>
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

    this->textHandler.init(coreHandle, 8, "assets/font/emulogic.ttf");
}

void MenuGame::startGettingText()
{
    this->coreHandle->startTextInput();
    this->isGettingText = true;
}

void MenuGame::endGettingText()
{
    this->coreHandle->endTextInput();
    this->isGettingText = false;
}

void MenuGame::handleNormalInput()
{
    if (this->coreHandle->isButtonPressed(ICore::Button::A) ||
        this->coreHandle->isButtonPressed(ICore::Button::Start)) {
        this->coreHandle->menuNotifyIsFinished = true;
        return;
    }
    if (this->coreHandle->isButtonPressed(ICore::Button::Left))
        this->coreHandle->decrementCurrentlySelectedGame();
    if (this->coreHandle->isButtonPressed(ICore::Button::Right))
        this->coreHandle->incrementCurrentlySelectedGame();
    if (this->coreHandle->isButtonPressed(ICore::Button::Up))
        this->coreHandle->decrementCurrentlySelectedDisplay();
    if (this->coreHandle->isButtonPressed(ICore::Button::Down))
        this->coreHandle->incrementCurrentlySelectedDisplay();

    auto mouseReleaseEvent = this->coreHandle->getMouseButtonReleaseEvent();
    if (mouseReleaseEvent.type != ICore::MouseButtonReleaseEvent::Type::None) {
        if (std::clamp(mouseReleaseEvent.cellPosition.x, std::uint32_t(40), std::uint32_t(60)) == mouseReleaseEvent.cellPosition.x &&
            std::clamp(mouseReleaseEvent.cellPosition.y, std::uint32_t(3), std::uint32_t(37)) == mouseReleaseEvent.cellPosition.y) {
            this->startGettingText();
            return;
        }
    }
}

inline std::string convertBackspaces(std::string str)
{
    for (auto it = str.begin(); it != str.end(); ++it) {
        it = std::find(it, str.end(), '\b');
        if (it == str.end())
            break;
        if (it == str.begin())
            it = str.erase(it);
        else
            it = str.erase(it - 1, it + 1);
    }
    return str;
}

void MenuGame::handleTextInput()
{
    this->coreHandle->playerName += this->coreHandle->getTextInput();
    this->coreHandle->playerName = convertBackspaces(this->coreHandle->playerName);
    if (this->coreHandle->playerName.find('\n') != std::string::npos) {
        this->coreHandle->playerName.erase(this->coreHandle->playerName.find('\n'));
        this->endGettingText();
        return;
    }

    auto mouseReleaseEvent = this->coreHandle->getMouseButtonReleaseEvent();
    if (mouseReleaseEvent.type != ICore::MouseButtonReleaseEvent::Type::None)
        if (std::clamp(mouseReleaseEvent.cellPosition.x, std::uint32_t(60), std::uint32_t(80)) != mouseReleaseEvent.cellPosition.x ||
            std::clamp(mouseReleaseEvent.cellPosition.y, std::uint32_t(3), std::uint32_t(37)) != mouseReleaseEvent.cellPosition.y)
            this->endGettingText();
}

void MenuGame::update()
{
    if (!this->isGettingText)
        this->handleNormalInput();
    else
        this->handleTextInput();
}

void MenuGame::draw()
{
    this->coreHandle->clearScreen(ICore::Color::black);
    this->textHandler.drawText("D-Pad -> WASD, ABXY -> Arrow Keys, L & R -> Q and E, Start & Select -> C and V", std::numeric_limits<std::size_t>::max(), utils::posCellToPix({0, 0}, 8));
    this->textHandler.drawText("Menu: Left/Right changes games, Up/Down changes display, a/start begins", std::numeric_limits<std::size_t>::max(), utils::posCellToPix({0, 1}, 8));
    this->textHandler.drawText("F1/F2: change display lib, F3/F4: change game, F5: restart, F6: menu, F7: exit", std::numeric_limits<std::size_t>::max(), utils::posCellToPix({0, 2}, 8));
    this->drawBox({0, 3}, {20, 37});
    this->drawBox({20, 3}, {20, 37});
    this->drawBox({40, 3}, {20, 37});
    this->drawBox({60, 3}, {20, 37});

    for (std::uint32_t i = 0; i < this->coreHandle->getGameLibraries().size() && i < 35; ++i) {
        this->textHandler.drawText(this->coreHandle->getGameLibraries()[i].first.getFileName().substr(4), 17, utils::posCellToPix({2, 4 + i}, 8));
        if (this->coreHandle->currentlySelectedGame == i)
            this->textHandler.drawText(">", 1, utils::posCellToPix({1, 4 + i}, 8));
    }

    for (std::uint32_t i = 0; i < this->coreHandle->getDisplayLibraries().size() && i < 35; ++i) {
        this->textHandler.drawText(this->coreHandle->getDisplayLibraries()[i].first.getFileName().substr(4), 17, utils::posCellToPix({22, 4 + i}, 8));
        if (this->coreHandle->currentlySelectedDisplay == i)
            this->textHandler.drawText(">", 1, utils::posCellToPix({21, 4 + i}, 8));
    }

    std::uint32_t scoresPrinted = 0;
    for (auto it = this->coreHandle->getScores().rbegin(); it != this->coreHandle->getScores().rend() && scoresPrinted < 35; ++it, ++scoresPrinted) {
        this->textHandler.drawText(it->playerName, 10, utils::posCellToPix({61, 4 + scoresPrinted}, 8));
        this->coreHandle->renderSprite({utils::posCellToPix({71, 4 + scoresPrinted}, 8), this->boxLeft});

        std::string scoreString = std::string(7, '0') + std::to_string(it->value);
        scoreString = scoreString.substr(scoreString.size() - 7);
        this->textHandler.drawText(scoreString, 7, utils::posCellToPix({72, 4 + scoresPrinted}, 8));
    }

    this->textHandler.drawText("Enter name here:", std::numeric_limits<std::size_t>::max(), utils::posCellToPix({41, 4}, 8));
    this->textHandler.drawText(this->coreHandle->playerName, 18, utils::posCellToPix({41, 8}, 8));
}

void MenuGame::drawBox(ICore::Vector2u cellPosition, ICore::Vector2u boxSize)
{
    assert(boxSize.x > 1 && boxSize.y > 1);
    this->coreHandle->renderSprite({utils::posCellToPix(cellPosition, 8), this->boxTopLeft});
    this->coreHandle->renderSprite({utils::posCellToPix({cellPosition.x + boxSize.x - 1, cellPosition.y}, 8), this->boxTopRight});
    this->coreHandle->renderSprite({utils::posCellToPix({cellPosition.x, cellPosition.y + boxSize.y - 1}, 8), this->boxBottomLeft});
    this->coreHandle->renderSprite({utils::posCellToPix({cellPosition.x + boxSize.x - 1, cellPosition.y + boxSize.y - 1}, 8), this->boxBottomRight});
    for (std::uint32_t i = 1; i < (boxSize.x - 1); ++i) {
        this->coreHandle->renderSprite({utils::posCellToPix({cellPosition.x + i, cellPosition.y}, 8), this->boxTop});
        this->coreHandle->renderSprite({utils::posCellToPix({cellPosition.x + i, cellPosition.y + boxSize.y - 1}, 8), this->boxBottom});
    }
    for (std::uint32_t i = 1; i < (boxSize.y - 1); ++i) {
        this->coreHandle->renderSprite({utils::posCellToPix({cellPosition.x, cellPosition.y + i}, 8), this->boxLeft});
        this->coreHandle->renderSprite({utils::posCellToPix({cellPosition.x + boxSize.x - 1, cellPosition.y + i}, 8), this->boxRight});
    }
}
