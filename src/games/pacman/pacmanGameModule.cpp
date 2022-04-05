#include "pacmanGameModule.hpp"
#include "utils/positionConverters.hpp"
#include "utils/vector2u.hpp"
#include <exception>
#include <functional>
#include <limits>
#include <cassert>

constexpr bool originalGameBehavior = false; // Whether we should try to follow with the original game as best as possible or do what we got ordered to do
constexpr bool debugEnableAll = false;
constexpr bool debugGodMode = debugEnableAll || false;
constexpr bool debugFreeze = debugEnableAll || false;
constexpr bool debugTargets = debugEnableAll || false;
constexpr std::uint32_t debugStartingRound = 0;

PacmanGameModule::GameState::LevelInformation PacmanGameModule::GameState::getLevelInformation(std::uint32_t round)
{
    round = std::min(static_cast<std::size_t>(round), std::size(GameState::levelInformationTable) - 1);
    return GameState::levelInformationTable[round];
}

ICore::Vector2u PacmanGameModule::GameState::directionToVector2u(PacmanGameModule::GameState::Direction direction)
{
    assert(!(direction < 0) && !(direction >= PacmanGameModule::GameState::dirCount));

    constexpr ICore::Vector2u mapDirectionsToVector2u[] = {
        [PacmanGameModule::GameState::dirRight] = {1, 0},
        [PacmanGameModule::GameState::dirDown] = {0, 1},
        [PacmanGameModule::GameState::dirLeft] = {static_cast<std::uint32_t>(-1), 0},
        [PacmanGameModule::GameState::dirUp] = {0, static_cast<std::uint32_t>(-1)},
    };
    return mapDirectionsToVector2u[direction];
}

PacmanGameModule::GameState::Direction PacmanGameModule::GameState::directionReverse(GameState::Direction direction)
{
    switch (direction) {
    case GameState::dirRight:
        return GameState::dirLeft;
    case GameState::dirDown:
        return GameState::dirUp;
    case GameState::dirLeft:
        return GameState::dirRight;
    case GameState::dirUp:
        return GameState::dirDown;
    default:
        return GameState::dirDown;
    }
}

bool PacmanGameModule::GameState::cellPositionIsInTunnel(ICore::Vector2u cellPosition)
{
    return ((cellPosition.x <= 5) || (cellPosition.x >= 22)) && (cellPosition.y == 17);
}

bool PacmanGameModule::GameState::cellPositionIsInRedzone(ICore::Vector2u cellPosition)
{
    return (cellPosition.x >= 11) && (cellPosition.x <= 16) && ((cellPosition.y == 14) || (cellPosition.y == 26));
}

ICore::Vector2u PacmanGameModule::distanceToCellMiddle(ICore::Vector2u pixelPosition)
{
    return {
        (8 / 2) - pixelPosition.x % 8,
        (8 / 2) - pixelPosition.y % 8,
    };
}

ICore::Vector2u PacmanGameModule::clampCellPosition(ICore::Vector2u cellPosition)
{
    if (static_cast<std::int32_t>(cellPosition.x) < 0)
        cellPosition.x = 0;
    if (cellPosition.x >= PacmanGameModule::displayCells.x)
        cellPosition.x = PacmanGameModule::displayCells.x - 1;
    if (static_cast<std::int32_t>(cellPosition.y) < 3)
        cellPosition.y = 3;
    if (cellPosition.y >= PacmanGameModule::displayCells.y - 2)
        cellPosition.y = PacmanGameModule::displayCells.y - 3;
    return cellPosition;
}

ICore::Vector2u PacmanGameModule::actorPositionToSpritePosition(ICore::Vector2u actorPosition)
{
    return {
        actorPosition.x - (16 / 2),
        actorPosition.y - (16 / 2),
    };
}

void PacmanGameModule::init(ICore *coreHandle)
{
    this->coreHandle = coreHandle;

    this->coreHandle->setPixelsPerCell(8);
    this->coreHandle->openWindow({PacmanGameModule::displayPixels.x, PacmanGameModule::displayPixels.y});
    this->textHandler.init(this->coreHandle, 8, "assets/font/emulogic.ttf");
    this->textureBlinkyFaceRight1 = this->coreHandle->loadTexture("assets/pacman/blinky-face-right-1.png", 'R', ICore::Color::red, ICore::Color::black, 16, 16);
    this->texturePinkyFaceRight1 = this->coreHandle->loadTexture("assets/pacman/pinky-face-right-1.png", 'R', ICore::Color::magenta, ICore::Color::black, 16, 16);
    this->textureInkyFaceRight1 = this->coreHandle->loadTexture("assets/pacman/inky-face-right-1.png", 'R', ICore::Color::cyan, ICore::Color::black, 16, 16);
    this->textureClydeFaceRight1 = this->coreHandle->loadTexture("assets/pacman/clyde-face-right-1.png", 'R', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->textureDot = this->coreHandle->loadTexture("assets/pacman/dot.png", '.', ICore::Color::white, ICore::Color::black, 8, 8);
    this->texturePill = this->coreHandle->loadTexture("assets/pacman/pill.png", '*', ICore::Color::white, ICore::Color::black, 8, 8);
    this->textureMazeBoxBottomLeft = this->coreHandle->loadTexture("assets/pacman/maze-box-bottom-left.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxBottom = this->coreHandle->loadTexture("assets/pacman/maze-box-bottom.png", '-', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxBottomRight = this->coreHandle->loadTexture("assets/pacman/maze-box-bottom-right.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxLeft = this->coreHandle->loadTexture("assets/pacman/maze-box-left.png", '|', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxRight = this->coreHandle->loadTexture("assets/pacman/maze-box-right.png", '|', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxTopLeft = this->coreHandle->loadTexture("assets/pacman/maze-box-top-left.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxTop = this->coreHandle->loadTexture("assets/pacman/maze-box-top.png", '-', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxTopRight = this->coreHandle->loadTexture("assets/pacman/maze-box-top-right.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeLeft = this->coreHandle->loadTexture("assets/pacman/maze-left.png", '|', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeRight = this->coreHandle->loadTexture("assets/pacman/maze-right.png", '|', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeTopLeft = this->coreHandle->loadTexture("assets/pacman/maze-top-left.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeTopMiddleLeft = this->coreHandle->loadTexture("assets/pacman/maze-top-middle-left.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeTopMiddleRight = this->coreHandle->loadTexture("assets/pacman/maze-top-middle-right.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeTop = this->coreHandle->loadTexture("assets/pacman/maze-top.png", '-', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeTopRight = this->coreHandle->loadTexture("assets/pacman/maze-top-right.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxBottomToLeftCorner = this->coreHandle->loadTexture("assets/pacman/maze-box-bottom-to-left-corner.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxBottomToRightCorner = this->coreHandle->loadTexture("assets/pacman/maze-box-bottom-to-right-corner.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxTopLeft = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-top-left.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxTop = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-top.png", '-', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxEntranceLeft = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-entrance-left.png", '|', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxEntranceRight = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-entrance-right.png", '|', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxEntranceMiddle = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-entrance-middle.png", '-', ICore::Color::yellow, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxTopRight = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-top-right.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxLeft = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-left.png", '|', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxRight = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-right.png", '|', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxBottom = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-bottom.png", '-', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxBottomLeft = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-bottom-left.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeMiddleBoxBottomRight = this->coreHandle->loadTexture("assets/pacman/maze-middle-box-bottom-right.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBottom = this->coreHandle->loadTexture("assets/pacman/maze-bottom.png", '-', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeRightMiddleTop = this->coreHandle->loadTexture("assets/pacman/maze-right-middle-top.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeRightMiddleBottom = this->coreHandle->loadTexture("assets/pacman/maze-right-middle-bottom.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeLeftMiddleTop = this->coreHandle->loadTexture("assets/pacman/maze-left-middle-top.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeLeftMiddleBottom = this->coreHandle->loadTexture("assets/pacman/maze-left-middle-bottom.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxTopToLeftCorner = this->coreHandle->loadTexture("assets/pacman/maze-box-top-to-left-corner.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBoxTopToRightCorner = this->coreHandle->loadTexture("assets/pacman/maze-box-top-to-right-corner.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBottomLeft = this->coreHandle->loadTexture("assets/pacman/maze-bottom-left.png", '\\', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureMazeBottomRight = this->coreHandle->loadTexture("assets/pacman/maze-bottom-right.png", '/', ICore::Color::blue, ICore::Color::black, 8, 8);
    this->textureFruitApple = this->coreHandle->loadTexture("assets/pacman/fruit-apple.png", '-', ICore::Color::red, ICore::Color::black, 16, 16);
    this->textureFruitBell = this->coreHandle->loadTexture("assets/pacman/fruit-bell.png", '|', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->textureFruitCherries = this->coreHandle->loadTexture("assets/pacman/fruit-cherries.png", '_', ICore::Color::red, ICore::Color::black, 16, 16);
    this->textureFruitGalaxian = this->coreHandle->loadTexture("assets/pacman/fruit-galaxian.png", '^', ICore::Color::blue, ICore::Color::black, 16, 16);
    this->textureFruitGrapes = this->coreHandle->loadTexture("assets/pacman/fruit-grapes.png", '/', ICore::Color::green, ICore::Color::black, 16, 16);
    this->textureFruitKey = this->coreHandle->loadTexture("assets/pacman/fruit-key.png", '|', ICore::Color::cyan, ICore::Color::black, 16, 16);
    this->textureFruitPeach = this->coreHandle->loadTexture("assets/pacman/fruit-peach.png", '-', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->textureFruitStrawberry = this->coreHandle->loadTexture("assets/pacman/fruit-strawberry.png", '-', ICore::Color::white, ICore::Color::black, 16, 16);
    this->texturePacmanFaceClosed = this->coreHandle->loadTexture("assets/pacman/pacman-face-closed.png", '-', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath1 = this->coreHandle->loadTexture("assets/pacman/pacman-death-1.png", 'v', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath2 = this->coreHandle->loadTexture("assets/pacman/pacman-death-2.png", 'v', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath3 = this->coreHandle->loadTexture("assets/pacman/pacman-death-3.png", '-', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath4 = this->coreHandle->loadTexture("assets/pacman/pacman-death-4.png", '-', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath5 = this->coreHandle->loadTexture("assets/pacman/pacman-death-5.png", '-', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath6 = this->coreHandle->loadTexture("assets/pacman/pacman-death-6.png", '_', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath7 = this->coreHandle->loadTexture("assets/pacman/pacman-death-7.png", '_', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath8 = this->coreHandle->loadTexture("assets/pacman/pacman-death-8.png", '|', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath9 = this->coreHandle->loadTexture("assets/pacman/pacman-death-9.png", '|', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath10 = this->coreHandle->loadTexture("assets/pacman/pacman-death-10.png", '|', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanDeath11 = this->coreHandle->loadTexture("assets/pacman/pacman-death-11.png", '*', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanFaceDown1 = this->coreHandle->loadTexture("assets/pacman/pacman-face-down-1.png", '^', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanFaceDown2 = this->coreHandle->loadTexture("assets/pacman/pacman-face-down-2.png", '^', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanFaceLeft1 = this->coreHandle->loadTexture("assets/pacman/pacman-face-left-1.png", '>', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanFaceLeft2 = this->coreHandle->loadTexture("assets/pacman/pacman-face-left-2.png", '>', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanFaceRight1 = this->coreHandle->loadTexture("assets/pacman/pacman-face-right-1.png", '<', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanFaceRight2 = this->coreHandle->loadTexture("assets/pacman/pacman-face-right-2.png", '<', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanFaceUp1 = this->coreHandle->loadTexture("assets/pacman/pacman-face-up-1.png", 'v', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->texturePacmanFaceUp2 = this->coreHandle->loadTexture("assets/pacman/pacman-face-up-2.png", 'v', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->textureGhostEyesDown = this->coreHandle->loadTexture("assets/pacman/ghost-eyes-down.png", 'v', ICore::Color::blue, ICore::Color::black, 16, 16);
    this->textureGhostEyesLeft = this->coreHandle->loadTexture("assets/pacman/ghost-eyes-left.png", '<', ICore::Color::blue, ICore::Color::black, 16, 16);
    this->textureGhostEyesRight = this->coreHandle->loadTexture("assets/pacman/ghost-eyes-right.png", '>', ICore::Color::blue, ICore::Color::black, 16, 16);
    this->textureGhostEyesUp = this->coreHandle->loadTexture("assets/pacman/ghost-eyes-up.png", '^', ICore::Color::blue, ICore::Color::black, 16, 16);
    this->textureGhostFrightenedBlue1 = this->coreHandle->loadTexture("assets/pacman/ghost-frightened-blue-1.png", '-', ICore::Color::blue, ICore::Color::black, 16, 16);
    this->textureGhostFrightenedBlue2 = this->coreHandle->loadTexture("assets/pacman/ghost-frightened-blue-2.png", '-', ICore::Color::blue, ICore::Color::black, 16, 16);
    this->textureGhostFrightenedWhite1 = this->coreHandle->loadTexture("assets/pacman/ghost-frightened-white-1.png", '-', ICore::Color::white, ICore::Color::black, 16, 16);
    this->textureGhostFrightenedWhite2 = this->coreHandle->loadTexture("assets/pacman/ghost-frightened-white-2.png", '-', ICore::Color::white, ICore::Color::black, 16, 16);
    this->textureBlinkyFaceRight2 = this->coreHandle->loadTexture("assets/pacman/blinky-face-right-2.png", 'R', ICore::Color::red, ICore::Color::black, 16, 16);
    this->textureBlinkyFaceDown1 = this->coreHandle->loadTexture("assets/pacman/blinky-face-down-1.png", 'D', ICore::Color::red, ICore::Color::black, 16, 16);
    this->textureBlinkyFaceDown2 = this->coreHandle->loadTexture("assets/pacman/blinky-face-down-2.png", 'D', ICore::Color::red, ICore::Color::black, 16, 16);
    this->textureBlinkyFaceLeft1 = this->coreHandle->loadTexture("assets/pacman/blinky-face-left-1.png", 'L', ICore::Color::red, ICore::Color::black, 16, 16);
    this->textureBlinkyFaceLeft2 = this->coreHandle->loadTexture("assets/pacman/blinky-face-left-2.png", 'L', ICore::Color::red, ICore::Color::black, 16, 16);
    this->textureBlinkyFaceUp1 = this->coreHandle->loadTexture("assets/pacman/blinky-face-up-1.png", 'U', ICore::Color::red, ICore::Color::black, 16, 16);
    this->textureBlinkyFaceUp2 = this->coreHandle->loadTexture("assets/pacman/blinky-face-up-2.png", 'U', ICore::Color::red, ICore::Color::black, 16, 16);
    this->texturePinkyFaceRight2 = this->coreHandle->loadTexture("assets/pacman/pinky-face-right-2.png", 'R', ICore::Color::magenta, ICore::Color::black, 16, 16);
    this->texturePinkyFaceDown1 = this->coreHandle->loadTexture("assets/pacman/pinky-face-down-1.png", 'D', ICore::Color::magenta, ICore::Color::black, 16, 16);
    this->texturePinkyFaceDown2 = this->coreHandle->loadTexture("assets/pacman/pinky-face-down-2.png", 'D', ICore::Color::magenta, ICore::Color::black, 16, 16);
    this->texturePinkyFaceLeft1 = this->coreHandle->loadTexture("assets/pacman/pinky-face-left-1.png", 'L', ICore::Color::magenta, ICore::Color::black, 16, 16);
    this->texturePinkyFaceLeft2 = this->coreHandle->loadTexture("assets/pacman/pinky-face-left-2.png", 'L', ICore::Color::magenta, ICore::Color::black, 16, 16);
    this->texturePinkyFaceUp1 = this->coreHandle->loadTexture("assets/pacman/pinky-face-up-1.png", 'U', ICore::Color::magenta, ICore::Color::black, 16, 16);
    this->texturePinkyFaceUp2 = this->coreHandle->loadTexture("assets/pacman/pinky-face-up-2.png", 'U', ICore::Color::magenta, ICore::Color::black, 16, 16);
    this->textureInkyFaceRight2 = this->coreHandle->loadTexture("assets/pacman/inky-face-right-2.png", 'R', ICore::Color::cyan, ICore::Color::black, 16, 16);
    this->textureInkyFaceDown1 = this->coreHandle->loadTexture("assets/pacman/inky-face-down-1.png", 'D', ICore::Color::cyan, ICore::Color::black, 16, 16);
    this->textureInkyFaceDown2 = this->coreHandle->loadTexture("assets/pacman/inky-face-down-2.png", 'D', ICore::Color::cyan, ICore::Color::black, 16, 16);
    this->textureInkyFaceLeft1 = this->coreHandle->loadTexture("assets/pacman/inky-face-left-1.png", 'L', ICore::Color::cyan, ICore::Color::black, 16, 16);
    this->textureInkyFaceLeft2 = this->coreHandle->loadTexture("assets/pacman/inky-face-left-2.png", 'L', ICore::Color::cyan, ICore::Color::black, 16, 16);
    this->textureInkyFaceUp1 = this->coreHandle->loadTexture("assets/pacman/inky-face-up-1.png", 'U', ICore::Color::cyan, ICore::Color::black, 16, 16);
    this->textureInkyFaceUp2 = this->coreHandle->loadTexture("assets/pacman/inky-face-up-2.png", 'U', ICore::Color::cyan, ICore::Color::black, 16, 16);
    this->textureClydeFaceRight2 = this->coreHandle->loadTexture("assets/pacman/clyde-face-right-2.png", 'R', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->textureClydeFaceDown1 = this->coreHandle->loadTexture("assets/pacman/clyde-face-down-1.png", 'D', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->textureClydeFaceDown2 = this->coreHandle->loadTexture("assets/pacman/clyde-face-down-2.png", 'D', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->textureClydeFaceLeft1 = this->coreHandle->loadTexture("assets/pacman/clyde-face-left-1.png", 'L', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->textureClydeFaceLeft2 = this->coreHandle->loadTexture("assets/pacman/clyde-face-left-2.png", 'L', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->textureClydeFaceUp1 = this->coreHandle->loadTexture("assets/pacman/clyde-face-up-1.png", 'U', ICore::Color::yellow, ICore::Color::black, 16, 16);
    this->textureClydeFaceUp2 = this->coreHandle->loadTexture("assets/pacman/clyde-face-up-2.png", 'U', ICore::Color::yellow, ICore::Color::black, 16, 16);

    this->introState.triggerStart.setNow(this);
}

void PacmanGameModule::resetAllDraw()
{
    this->drawnCellThings.clear();
}

bool PacmanGameModule::isButtonPressed(ICore::Button button)
{
    if (!this->inputEnabled)
        return false;
    return this->coreHandle->isButtonPressed(button);
}

bool PacmanGameModule::isValidCell(ICore::Vector2u cellPosition)
{
    return (cellPosition.x < PacmanGameModule::displayCells.x && cellPosition.y < PacmanGameModule::displayCells.y);
}

void PacmanGameModule::setCellCharacter(ICore::Vector2u cellPosition, char character, ICore::Color color)
{
    assert(this->isValidCell(cellPosition));
    this->drawnCellThings[cellPosition] = {DrawableCellCharacter{character, color}, true};
}

void PacmanGameModule::setCellSprite(ICore::Vector2u cellPosition, ICore::Texture *texture, ICore::Vector2u realPositionOffset)
{
    assert(this->isValidCell(cellPosition));
    assert(texture != nullptr);
    this->drawnCellThings[cellPosition] = {DrawableCellTexture{texture, realPositionOffset}, true};
}

void PacmanGameModule::setCellText(ICore::Vector2u cellPosition, std::string_view text, ICore::Color color)
{
    assert(this->isValidCell(cellPosition));

    for (auto i : text) {
        if (cellPosition.x < PacmanGameModule::displayCells.x) {
            this->setCellCharacter(cellPosition, i, color);
            ++cellPosition.x;
        } else
            break;
    }
}

// The score is drawn from right to left
// Last printed number is always 0
// Score of 0 will print as "00", same as in the real Pac-Man
void PacmanGameModule::setCellScore(ICore::Vector2u cellPosition, std::uint64_t score)
{
    this->setCellCharacter(cellPosition, '0');
    --cellPosition.x;
    for (int i = 0; i < 8; ++i) {
        char character = '0' + (score % 10);
        if (this->isValidCell(cellPosition)) {
            this->setCellCharacter(cellPosition, character);
            --cellPosition.x;
            score /= 10;
            if (score == 0)
                break;
        }
    }
}

void PacmanGameModule::setCellScoreFruit(PacmanGameModule::GameState::BonusFruit bonusFruit)
{
    assert(
        static_cast<int>(bonusFruit) >= 0 &&
        static_cast<int>(bonusFruit) < static_cast<int>(PacmanGameModule::GameState::BonusFruit::amountOfBonusFruits));

    constexpr std::uint64_t mapFruitsToScore[] = {0, 10, 30, 50, 70, 100, 200, 300, 500};

    if (bonusFruit == PacmanGameModule::GameState::BonusFruit::none)
        this->setCellText({11, 20}, "    ");
    else
        this->setCellScore({14, 20}, mapFruitsToScore[static_cast<int>(bonusFruit)]);
}

void PacmanGameModule::setCellEnable(ICore::Vector2u cellPosition, bool enable)
{
    auto it = this->drawnCellThings.find(cellPosition);
    if (it != this->drawnCellThings.end())
        it->second.enabled = enable;
}

std::optional<PacmanGameModule::DrawableCellThing> PacmanGameModule::getCellDrawnThing(ICore::Vector2u cellPosition)
{
    auto result = this->drawnCellThings.find(cellPosition);
    if (result == this->drawnCellThings.end())
        return std::nullopt;
    return result->second;
}

void PacmanGameModule::clearSprites()
{
    for (auto &i : this->sprites)
        i.enabled = false;
}

ICore::Texture *PacmanGameModule::getBonusFruitTexture(PacmanGameModule::GameState::BonusFruit bonusFruit)
{
    switch (bonusFruit) {
    case GameState::BonusFruit::cherries:
        return this->textureFruitCherries;
    case GameState::BonusFruit::strawberry:
        return this->textureFruitStrawberry;
    case GameState::BonusFruit::peach:
        return this->textureFruitPeach;
    case GameState::BonusFruit::apple:
        return this->textureFruitApple;
    case GameState::BonusFruit::grapes:
        return this->textureFruitGrapes;
    case GameState::BonusFruit::galaxian:
        return this->textureFruitGalaxian;
    case GameState::BonusFruit::bell:
        return this->textureFruitBell;
    case GameState::BonusFruit::key:
        return this->textureFruitKey;
    default:
        return nullptr;
    };
}

void PacmanGameModule::updateIntro()
{
    // When we start up the intro, draw the initial text
    if (this->introState.triggerStart.isNow(this)) {
        this->soundHandler.stopAllSounds();
        this->clearSprites();
        this->resetAllDraw();
        this->inputEnabled = true;
        this->setCellText({3, 0}, "1UP   HIGH SCORE   2UP");
        this->setCellScore({6, 1}, 0);
        this->setCellText({7, 5}, "CHARACTER / NICKNAME");
        this->setCellText({3, 35}, "CREDIT  0");
        if (this->gameState.highScore != 0)
            this->setCellScore({16, 1}, this->gameState.highScore);
    }

    std::uint64_t totalDelay = 30;
    constexpr struct GhostInfo {
        std::string_view nickname;
        std::string_view name;
        ICore::Color color;
        ICore::Texture *PacmanGameModule::* texture;
    } ghostsInfo[] = {
        {"-SHADOW", "\"BLINKY\"", ICore::Color::red, &PacmanGameModule::textureBlinkyFaceRight1},
        {"-SPEEDY", "\"PINKY\"", ICore::Color::magenta, &PacmanGameModule::texturePinkyFaceRight1},
        {"-BASHFUL", "\"INKY\"", ICore::Color::cyan, &PacmanGameModule::textureInkyFaceRight1},
        {"-POKEY", "\"CLYDE\"", ICore::Color::yellow, &PacmanGameModule::textureClydeFaceRight1}
    };

    for (std::size_t i = 0; i < std::size(ghostsInfo); ++i) {
        const std::uint32_t yPos = 3 * i + 7;

        // After 0.5 seconds, the ghost appears
        totalDelay += 30;
        if (this->introState.triggerStart.isExactlyAfter(this, totalDelay))
            this->setCellSprite({4, yPos}, this->*ghostsInfo[i].texture, {0, -4u});

        // After 1 more second, the ghost's nickname appears
        totalDelay += 60;
        if (this->introState.triggerStart.isExactlyAfter(this, totalDelay))
            this->setCellText({7, yPos}, ghostsInfo[i].nickname, ghostsInfo[i].color);

        // After 0.5 more seconds, the ghost's name appears
        totalDelay += 30;
        if (this->introState.triggerStart.isExactlyAfter(this, totalDelay))
            this->setCellText({17, yPos}, ghostsInfo[i].name, ghostsInfo[i].color);
    }

    // After 1 more second, the dots score info stuff appears
    totalDelay += 60;
    if (this->introState.triggerStart.isExactlyAfter(this, totalDelay)) {
        this->setCellSprite({10, 24}, this->textureDot);
        this->setCellSprite({10, 26}, this->texturePill);
        this->setCellText({12, 24}, "10 pts");
        this->setCellText({12, 26}, "50 pts");
    }

    // After 1 more second, we start displaying "PRESS ANY KEY TO START!"
    totalDelay += 60;
    if (this->introState.triggerStart.isAfter(this, totalDelay)) {
        if ((this->introState.triggerStart.framesSince(this) % 60) > 30)
            this->setCellText({1, 31}, "                          ");
        else
            this->setCellText({1, 31}, "PRESS ANY BUTTON TO START!");
    }

    // Fuck it I'm not doing the chase sequence rn (TODO)

    ICore::Button buttonList[] = {
        ICore::Button::Left,
        ICore::Button::Right,
        ICore::Button::Up,
        ICore::Button::Down,
        ICore::Button::A,
        ICore::Button::B,
        ICore::Button::X,
        ICore::Button::Y,
        ICore::Button::L,
        ICore::Button::R,
        ICore::Button::Start,
        ICore::Button::Select,
    };
    for (auto i : buttonList)
        if (this->isButtonPressed(i)) {
            this->inputEnabled = false;
            this->gameState.triggerStart.setNow(this);
        }
}

void PacmanGameModule::updateGameDisableAllTimers()
{
    this->gameState.triggerRoundWon.disable();
    this->gameState.triggerGameOver.disable();
    this->gameState.triggerAteDot.disable();
    this->gameState.triggerAtePill.disable();
    this->gameState.triggerAteGhost.disable();
    this->gameState.triggerPacmanKilled.disable();
    this->gameState.triggerAteBonusFruit.disable();
    this->gameState.triggerHouseLeaveForce.disable();
    this->gameState.triggerActivateBonusFruit.disable();
}

void PacmanGameModule::updateGameInitMaze()
{
    constexpr std::string_view mazeStr =
        "0111111111111231111111111114"
	"5............LR............6"
	"5.WTTX.WTTTX.LR.WTTTX.WTTX.6"
	"5PL  R.L   R.LR.L   R.L  RP6"
        "5.YBBZ.YBBBZ.YZ.YBBBZ.YBBZ.6"
        "5..........................6"
        "5.WTTX.WX.WTTTTTTX.WX.WTTX.6"
        "5.YBBZ.LR.YBBKJBBZ.LR.YBBZ.6"
        "5......LR....LR....LR......6"
        "beeeeX.LITTX LR WTTHR.Weeeec"
        "     5.LJBBZ YZ YBBKR.6     "
        "     5.LR          LR.6     "
        "     5.LR ztw--sty LR.6     "
        "11111Z.YZ l      r YZ.Y11111"
        "      .   l      r   .      "
        "eeeeeX.WX l      r WX.Weeeee"
        "     5.LR xddddddf LR.6     "
        "     5.LR          LR.6     "
        "     5.LR WTTTTTTX LR.6     "
        "01111Z.YZ YBBKJBBZ YZ.Y11114"
        "5............LR............6"
        "5.WTTX.WTTTX.LR.WTTTX.WTTX.6"
        "5.YBKR.YBBBZ.YZ.YBBBZ.LJBZ.6"
        "5P..LR.......  .......LR..P6"
        "aTX.LR.WX.WTTTTTTX.WX.LR.WT8"
        "7BZ.YZ.LR.YBBKJBBZ.LR.YZ.YB9"
        "5......LR....LR....LR......6"
        "5.WTTTTHITTX.LR.WTTHITTTTX.6"
        "5.YBBBBBBBBZ.YZ.YBBBBBBBBZ.6"
        "5..........................6"
        "beeeeeeeeeeeeeeeeeeeeeeeeeec";

    static const std::unordered_map<char, ICore::Texture *PacmanGameModule::*> strMapping = {
        {'.', &PacmanGameModule::textureDot},
        {'P', &PacmanGameModule::texturePill},
        {'0', &PacmanGameModule::textureMazeTopLeft},
        {'1', &PacmanGameModule::textureMazeTop},
        {'2', &PacmanGameModule::textureMazeTopMiddleLeft},
        {'3', &PacmanGameModule::textureMazeTopMiddleRight},
        {'4', &PacmanGameModule::textureMazeTopRight},
        {'5', &PacmanGameModule::textureMazeLeft},
        {'6', &PacmanGameModule::textureMazeRight},
        {'L', &PacmanGameModule::textureMazeBoxLeft},
        {'R', &PacmanGameModule::textureMazeBoxRight},
        {'W', &PacmanGameModule::textureMazeBoxTopLeft},
        {'T', &PacmanGameModule::textureMazeBoxTop},
        {'X', &PacmanGameModule::textureMazeBoxTopRight},
        {'Y', &PacmanGameModule::textureMazeBoxBottomLeft},
        {'B', &PacmanGameModule::textureMazeBoxBottom},
        {'Z', &PacmanGameModule::textureMazeBoxBottomRight},
        {'K', &PacmanGameModule::textureMazeBoxBottomToLeftCorner},
        {'J', &PacmanGameModule::textureMazeBoxBottomToRightCorner},
        {'I', &PacmanGameModule::textureMazeBoxTopToRightCorner},
        {'H', &PacmanGameModule::textureMazeBoxTopToLeftCorner},
        {'z', &PacmanGameModule::textureMazeMiddleBoxTopLeft},
        {'t', &PacmanGameModule::textureMazeMiddleBoxTop},
        {'w', &PacmanGameModule::textureMazeMiddleBoxEntranceLeft},
        {'-', &PacmanGameModule::textureMazeMiddleBoxEntranceMiddle},
        {'s', &PacmanGameModule::textureMazeMiddleBoxEntranceRight},
        {'y', &PacmanGameModule::textureMazeMiddleBoxTopRight},
        {'l', &PacmanGameModule::textureMazeMiddleBoxLeft},
        {'r', &PacmanGameModule::textureMazeMiddleBoxRight},
        {'x', &PacmanGameModule::textureMazeMiddleBoxBottomLeft},
        {'d', &PacmanGameModule::textureMazeMiddleBoxBottom},
        {'f', &PacmanGameModule::textureMazeMiddleBoxBottomRight},
        {'e', &PacmanGameModule::textureMazeBottom},
        {'8', &PacmanGameModule::textureMazeRightMiddleTop},
        {'9', &PacmanGameModule::textureMazeRightMiddleBottom},
        {'a', &PacmanGameModule::textureMazeLeftMiddleTop},
        {'7', &PacmanGameModule::textureMazeLeftMiddleBottom},
        {'b', &PacmanGameModule::textureMazeBottomLeft},
        {'c', &PacmanGameModule::textureMazeBottomRight},
    };

    for (std::uint32_t i = 0, y = 3; y <= 33; ++y)
        for (std::uint32_t x = 0; x < 28; ++i, ++x) {
            if (mazeStr[i] == ' ')
                continue;
            auto newThing = strMapping.find(mazeStr[i]);
            if (newThing == strMapping.end())
                throw std::runtime_error(std::string("bruh: '") + mazeStr[i] + "'");
            this->setCellSprite({x, y}, this->*strMapping.at(mazeStr[i]));
        }
}

void PacmanGameModule::updateGameInit()
{
    this->inputEnabled = true;
    this->updateGameDisableAllTimers();
    this->gameState.currentRound = debugStartingRound;
    this->gameState.freezeReason = PacmanGameModule::GameState::freezeReasonPrelude;
    if (debugFreeze)
        puts("freeze set only prelude");
    this->gameState.currentLives = PacmanGameModule::GameState::initialLifeCount;
    this->gameState.globalDotCounter.active = false;
    this->gameState.globalDotCounter.value = 0;
    this->gameState.dotsEatenCount = 0;
    if (this->gameState.score != 0)
        this->coreHandle->addNewScore(this->gameState.score * 10);
    this->gameState.score = 0;

    this->drawnCellThings.clear();
    this->setCellText({9, 0}, "HIGH SCORE");
    this->updateGameInitMaze();
    this->setCellText({9, 14}, "PLAYER ONE", ICore::Color::cyan);
    this->setCellText({11, 20}, "READY!", ICore::Color::yellow);
}

// Make actors visible, remove the "PLAYER ONE" text and start a new life
void PacmanGameModule::updateGameInitRound()
{
    this->clearSprites();

    // This removes the "PLAYER ONE" text
    this->setCellText({9, 14}, "          ");

    if (this->gameState.dotsEatenCount == PacmanGameModule::GameState::totalDotCount) {
        // The previous round ended because all dots were eaten. Redraw the maze and reset the global dot counter
        this->gameState.dotsEatenCount = 0;
        ++this->gameState.currentRound;
        this->updateGameInitMaze();
        this->gameState.globalDotCounter.active = false;
    } else {
        // The previous round ended because Pac-Man died. Use the global dot counter to detect the moment at which the ghosts shall leave the ghost house (instead of the usual "per-ghost" dot counter)
        // (note: this check is done by comparing the life count to the initial life count, but obviously that isn't exactly the most exact way of doing that... it's what the original game does, though, so...)
        if (this->gameState.currentLives != PacmanGameModule::GameState::initialLifeCount) {
            this->gameState.globalDotCounter.active = true;
            this->gameState.globalDotCounter.value = 0;
        }
        --this->gameState.currentLives;
    }
    assert(!(this->gameState.currentLives < 0));

    this->gameState.activeBonusFruit = PacmanGameModule::GameState::BonusFruit::none;
    this->gameState.freezeReason = PacmanGameModule::GameState::freezeReasonReady;
    if (debugFreeze)
        puts("freeze set only ready");
    this->gameState.ghostsEatenCount = 0;
    this->updateGameDisableAllTimers();

    this->setCellText({11, 20}, "READY!", ICore::Color::yellow);

    this->gameState.triggerHouseLeaveForce.setNow(this);

    // Pac-Man just starts up running to the left lol
    this->gameState.pacman = {
        .position = {14 * 8, 26 * 8 + 4},
        .currentDir = PacmanGameModule::GameState::dirLeft,
    };
    this->spritePacman->enabled = true;

    // Blinky is outside of the ghost house, looking left, in scatter mode
    *this->gameState.blinky = {
        .actor = {
            .position = PacmanGameModule::GameState::ghostStartingPositions[PacmanGameModule::GameState::ghostBlinky],
            .currentDir = PacmanGameModule::GameState::dirLeft,
        },
        .type = PacmanGameModule::GameState::ghostBlinky,
        .state = PacmanGameModule::GameState::Ghost::State::scatter,
        .nextDir = PacmanGameModule::GameState::dirLeft,
        .targetCell = {},
        .dotCount = 0,
        .dotLimit = 0,
        .triggerFrightened = {},
        .triggerGotEaten = {},
    };
    this->gameState.blinky->triggerFrightened.disable();
    this->gameState.blinky->triggerGotEaten.disable();
    this->spriteBlinky->enabled = true;

    // Pinky is in the middle of the ghost house, looking down
    *this->gameState.pinky = {
        .actor = {
            .position = PacmanGameModule::GameState::ghostStartingPositions[PacmanGameModule::GameState::ghostPinky],
            .currentDir = PacmanGameModule::GameState::dirDown,
        },
        .type = PacmanGameModule::GameState::ghostPinky,
        .state = PacmanGameModule::GameState::Ghost::State::inGhostHouse,
        .nextDir = PacmanGameModule::GameState::dirDown,
        .targetCell = {},
        .dotCount = 0,
        .dotLimit = 0,
        .triggerFrightened = {},
        .triggerGotEaten = {},
    };
    this->gameState.pinky->triggerFrightened.disable();
    this->gameState.pinky->triggerGotEaten.disable();
    this->spritePinky->enabled = true;

    // Inky is on the left of the ghost house, looking up
    *this->gameState.inky = {
        .actor = {
            .position = PacmanGameModule::GameState::ghostStartingPositions[PacmanGameModule::GameState::ghostInky],
            .currentDir = PacmanGameModule::GameState::dirUp,
        },
        .type = PacmanGameModule::GameState::ghostInky,
        .state = PacmanGameModule::GameState::Ghost::State::inGhostHouse,
        .nextDir = PacmanGameModule::GameState::dirUp,
        .targetCell = {},
        .dotCount = 0,
        .dotLimit = 30, // This actually depends on the current round, but fuck it, TODO
        .triggerFrightened = {},
        .triggerGotEaten = {},
    };
    this->gameState.inky->triggerFrightened.disable();
    this->gameState.inky->triggerGotEaten.disable();
    this->spriteInky->enabled = true;

    // Clyde is on the right of the ghost house, looking up
    *this->gameState.clyde = {
        .actor = {
            .position = PacmanGameModule::GameState::ghostStartingPositions[PacmanGameModule::GameState::ghostClyde],
            .currentDir = PacmanGameModule::GameState::dirUp,
        },
        .type = PacmanGameModule::GameState::ghostClyde,
        .state = PacmanGameModule::GameState::Ghost::State::inGhostHouse,
        .nextDir = PacmanGameModule::GameState::dirUp,
        .targetCell = {},
        .dotCount = 0,
        .dotLimit = 60, // This actually depends on the current round, but fuck it, TODO
        .triggerFrightened = {},
        .triggerGotEaten = {},
    };
    this->gameState.clyde->triggerFrightened.disable();
    this->gameState.clyde->triggerGotEaten.disable();
    this->spriteClyde->enabled = true;
}

bool PacmanGameModule::updateGameShouldPacmanMove()
{
    // We stop for a frame when a dot has been eaten
    if (this->gameState.triggerAteDot.isNow(this))
        return false;

    // We stop for 3 frames when a pill has been eaten
    if (this->gameState.triggerAtePill.framesSince(this) < 3)
        return false;

    // Pac-Man goes slightly faster when ghosts are frightened
    for (auto &i : this->gameState.ghosts)
        if (i.state == GameState::Ghost::State::frightened) {
            if (this->gameState.currentRound == 0)
                return this->currentFrame % 10;
            if (this->gameState.currentRound < 4)
                return this->currentFrame % 20;
            return true;
        }

    // Pac-Man goes slightly slower at the early rounds, then goes faster as rounds progress and slightly slower after round 21
    if (this->gameState.currentRound == 0)
        return this->currentFrame % 5;
    if ((this->gameState.currentRound >= 1 && this->gameState.currentRound <= 3) ||
        this->gameState.currentRound >= 20)
        return this->currentFrame % 10;
    return true;
}

bool PacmanGameModule::updateGameIsCellDot(ICore::Vector2u cellPosition)
{
    auto cellThing = this->getCellDrawnThing(cellPosition);

    return (
        cellThing.has_value() &&
        std::holds_alternative<PacmanGameModule::DrawableCellTexture>(cellThing->thing) &&
        std::get<PacmanGameModule::DrawableCellTexture>(cellThing->thing).texture == this->textureDot);
}

bool PacmanGameModule::updateGameIsCellPill(ICore::Vector2u cellPosition)
{
    auto cellThing = this->getCellDrawnThing(cellPosition);

    return (
        cellThing.has_value() &&
        std::holds_alternative<PacmanGameModule::DrawableCellTexture>(cellThing->thing) &&
        std::get<PacmanGameModule::DrawableCellTexture>(cellThing->thing).texture == this->texturePill);
}

bool PacmanGameModule::updateGameIsCellBlocking(ICore::Vector2u cellPosition)
{
    auto cellThing = this->getCellDrawnThing(cellPosition);

    if (!cellThing.has_value() || !std::holds_alternative<PacmanGameModule::DrawableCellTexture>(cellThing->thing))
        return false;
    auto cellTexture = std::get<PacmanGameModule::DrawableCellTexture>(cellThing->thing).texture;

    const ICore::Texture *blockingCells[] = {
        this->textureMazeBoxBottomLeft,
        this->textureMazeBoxBottom,
        this->textureMazeBoxBottomRight,
        this->textureMazeBoxLeft,
        this->textureMazeBoxRight,
        this->textureMazeBoxTopLeft,
        this->textureMazeBoxTop,
        this->textureMazeBoxTopRight,
        this->textureMazeLeft,
        this->textureMazeRight,
        this->textureMazeTopLeft,
        this->textureMazeTopMiddleLeft,
        this->textureMazeTopMiddleRight,
        this->textureMazeTop,
        this->textureMazeTopRight,
        this->textureMazeBoxBottomToLeftCorner,
        this->textureMazeBoxBottomToRightCorner,
        this->textureMazeBoxTopToRightCorner,
        this->textureMazeBoxTopToLeftCorner,
        this->textureMazeMiddleBoxTopLeft,
        this->textureMazeMiddleBoxTop,
        this->textureMazeMiddleBoxEntranceLeft,
        this->textureMazeMiddleBoxEntranceRight,
        this->textureMazeMiddleBoxEntranceMiddle,
        this->textureMazeMiddleBoxTopRight,
        this->textureMazeMiddleBoxLeft,
        this->textureMazeMiddleBoxRight,
        this->textureMazeMiddleBoxBottom,
        this->textureMazeMiddleBoxBottomLeft,
        this->textureMazeMiddleBoxBottomRight,
        this->textureMazeBottom,
        this->textureMazeRightMiddleTop,
        this->textureMazeRightMiddleBottom,
        this->textureMazeLeftMiddleTop,
        this->textureMazeLeftMiddleBottom,
        this->textureMazeBottomLeft,
        this->textureMazeBottomRight,
    };

    for (const ICore::Texture *i : blockingCells) {
        if (i == cellTexture)
            return true;
    }
    return false;
}

bool PacmanGameModule::updateGameCanMoveTo(ICore::Vector2u currentPosition, PacmanGameModule::GameState::Direction wantedDirection, bool allowCornering)
{
    const auto directionVec2u = PacmanGameModule::GameState::directionToVector2u(wantedDirection);
    const auto distanceToCellMiddle = PacmanGameModule::distanceToCellMiddle(currentPosition);

    std::int32_t moveDistanceMiddle = distanceToCellMiddle.x;
    std::int32_t perpendicularDistanceMiddle = distanceToCellMiddle.y;

    if (directionVec2u.y != 0)
        std::swap(moveDistanceMiddle, perpendicularDistanceMiddle);

    const auto cellPosition = utils::posPixToCell(currentPosition, 8);
    const auto checkPosition = PacmanGameModule::clampCellPosition(utils::addVec2u(cellPosition, directionVec2u));
    const auto isBlocked = this->updateGameIsCellBlocking(checkPosition);

    return !((!allowCornering && perpendicularDistanceMiddle) || (isBlocked && (moveDistanceMiddle == 0)));
}

ICore::Vector2u PacmanGameModule::updateGameMove(ICore::Vector2u position, PacmanGameModule::GameState::Direction direction, bool allowCornering)
{
    const auto directionVec2u = PacmanGameModule::GameState::directionToVector2u(direction);

    position = utils::addVec2u(position, directionVec2u);

    if (allowCornering) {
        const auto distanceMiddle = PacmanGameModule::distanceToCellMiddle(position);
        if (directionVec2u.x) {
            if (static_cast<std::int32_t>(distanceMiddle.y) < 0)
                --position.y;
            else if (static_cast<std::int32_t>(distanceMiddle.y) > 0)
                ++position.y;
        } else if (directionVec2u.y) {
            if (static_cast<std::int32_t>(distanceMiddle.x) < 0)
                --position.x;
            else if (static_cast<std::int32_t>(distanceMiddle.x) > 0)
                ++position.x;
        }
    }

    if (static_cast<std::int32_t>(position.x) < 0)
        position.x = PacmanGameModule::displayPixels.x - 1;
    else if (position.x >= PacmanGameModule::displayPixels.x)
        position.x = 0;
    return position;
}

void PacmanGameModule::updateGameDotEaten()
{
    ++this->gameState.dotsEatenCount;
    if (this->gameState.dotsEatenCount == PacmanGameModule::GameState::totalDotCount) {
        this->gameState.triggerRoundWon.setNow(this);
        this->soundHandler.stopAllSounds();
    } else if ((this->gameState.dotsEatenCount == 70) || (this->gameState.dotsEatenCount == 170))
        this->gameState.triggerActivateBonusFruit.setNow(this); // The bonus fruit is shown after eating 70 or 170 dots

    // We alternate between the two munch sound effects
    if (this->gameState.dotsEatenCount % 2)
        this->soundHandler.startSound("assets/pacman/munch-1.wav", 2);
    else
        this->soundHandler.startSound("assets/pacman/munch-2.wav", 2);
}

void PacmanGameModule::updateGameGhosthouseDotCounters()
{
    // We use the global dot counter but only if Pac-Man lost a life
    // (note: this is reset when all the ghosts have left the house)
    if (this->gameState.globalDotCounter.active)
        ++this->gameState.globalDotCounter.value;
    else {
        // We otherwise use each ghost's personal dot counter to determine when they leave the ghost house
        for (auto &i : this->gameState.ghosts)
            if (i.dotCount < i.dotLimit) {
                ++i.dotCount;
                break;
            }
    }
}

// Ghosts do less scattering as rounds go on
PacmanGameModule::GameState::Ghost::State PacmanGameModule::updateGameGetGhostScatterOrChase()
{
    auto framesSinceRoundStarted = this->gameState.triggerRoundStarted.framesSince(this);

    if (this->gameState.currentRound > 4) {
        if (framesSinceRoundStarted < 5 * 60)
            return GameState::Ghost::State::scatter;
        if (framesSinceRoundStarted < 25 * 60)
            return GameState::Ghost::State::chase;
        if (framesSinceRoundStarted < 30 * 60)
            return GameState::Ghost::State::scatter;
        if (framesSinceRoundStarted < 50 * 60)
            return GameState::Ghost::State::chase;
        if (framesSinceRoundStarted < 55 * 60)
            return GameState::Ghost::State::scatter;
        if (framesSinceRoundStarted < (55 + 1037) * 60)
            return GameState::Ghost::State::chase;
        if (framesSinceRoundStarted < ((55 + 1037) * 60 + 1))
            return GameState::Ghost::State::scatter;
        return GameState::Ghost::State::chase;
    }
    if (this->gameState.currentRound > 0) {
        if (framesSinceRoundStarted < 7 * 60)
            return GameState::Ghost::State::scatter;
        if (framesSinceRoundStarted < 27 * 60)
            return GameState::Ghost::State::chase;
        if (framesSinceRoundStarted < 34 * 60)
            return GameState::Ghost::State::scatter;
        if (framesSinceRoundStarted < 54 * 60)
            return GameState::Ghost::State::chase;
        if (framesSinceRoundStarted < 59 * 60)
            return GameState::Ghost::State::scatter;
        if (framesSinceRoundStarted < (59 + 1033) * 60)
            return GameState::Ghost::State::chase;
        if (framesSinceRoundStarted < ((59 + 1033) * 60 + 1))
            return GameState::Ghost::State::scatter;
        return GameState::Ghost::State::chase;
    }
    if (framesSinceRoundStarted < 7 * 60)
        return GameState::Ghost::State::scatter;
    if (framesSinceRoundStarted < 27 * 60)
        return GameState::Ghost::State::chase;
    if (framesSinceRoundStarted < 34 * 60)
        return GameState::Ghost::State::scatter;
    if (framesSinceRoundStarted < 54 * 60)
        return GameState::Ghost::State::chase;
    if (framesSinceRoundStarted < 59 * 60)
        return GameState::Ghost::State::scatter;
    if (framesSinceRoundStarted < 79 * 60)
        return GameState::Ghost::State::chase;
    if (framesSinceRoundStarted < 84 * 60)
        return GameState::Ghost::State::scatter;
    return GameState::Ghost::State::chase;
}

void PacmanGameModule::updateGameGhostState(PacmanGameModule::GameState::Ghost *ghost)
{
    assert(ghost != nullptr);

    auto ghostNextState = ghost->state;

    switch (ghost->state) {
    case GameState::Ghost::State::justEyes:
        // We check if we've reached the front of the ghost house, in which case we go into the "enter ghost house" state
        // This check is made to be fuzzy since eye state ghosts move faster than a pixel per frame
        if (utils::almostEqVec2u(ghost->actor.position, GameState::ghostHousePixelEnterPoint, 1))
            ghostNextState = GameState::Ghost::State::enteringGhostHouse;
        break;

    case GameState::Ghost::State::enteringGhostHouse:
        // Ghosts instantly leave the ghost house right after finishing entering it and reaching their target position within it
        if (utils::almostEqVec2u(ghost->actor.position, GameState::ghostTargetPositionsInGhostHouse[ghost->type], 1))
            ghostNextState = GameState::Ghost::State::leavingGhostHouse;
        break;

    case GameState::Ghost::State::inGhostHouse:
        // Ghosts only are in this state when the round has just started
        // One ghost gets forced out of the ghost house each time Pac-Man hasn't eaten a dot for 4 seconds (or 3 when on round 5 or later)
        if (this->gameState.triggerHouseLeaveForce.isExactlyAfter(this, (this->gameState.currentRound < 4 ? 4 : 3) * 60)) {
            ghostNextState = GameState::Ghost::State::leavingGhostHouse;
            this->gameState.triggerHouseLeaveForce.setNow(this);
        } else if (this->gameState.globalDotCounter.active) {
            // Pac-Man has already lost a life so the ghost's local dot counters potentially aren't really reliable anymore
            if ((ghost->type == GameState::ghostPinky) && (this->gameState.globalDotCounter.value == 7))
                ghostNextState = GameState::Ghost::State::leavingGhostHouse;
            if ((ghost->type == GameState::ghostInky) && (this->gameState.globalDotCounter.value == 17))
                ghostNextState = GameState::Ghost::State::leavingGhostHouse;
            if ((ghost->type == GameState::ghostClyde) && (this->gameState.globalDotCounter.value == 32)) {
                ghostNextState = GameState::Ghost::State::leavingGhostHouse;
                this->gameState.globalDotCounter.active = false;
            }
        } else if (ghost->dotCount == ghost->dotLimit)
            ghostNextState = GameState::Ghost::State::leavingGhostHouse;
        break;

    case GameState::Ghost::State::leavingGhostHouse:
        // Ghosts instantly go to scatter mode once they're done leaving the ghost house
        if (ghost->actor.position.y == GameState::ghostHousePixelEnterPoint.y)
            ghostNextState = GameState::Ghost::State::scatter;
        break;

    case GameState::Ghost::State::scatter:
    case GameState::Ghost::State::chase:
    case GameState::Ghost::State::frightened:
        // Handle switching in between frightened/scatter/chase
        if (ghost->triggerFrightened.isBefore(this, GameState::getLevelInformation(this->gameState.currentRound).afraidFrames))
            ghostNextState = GameState::Ghost::State::frightened;
        else
            ghostNextState = this->updateGameGetGhostScatterOrChase();
        break;

    default:
        assert(false && "WTF ????");
    }

    if (ghostNextState != ghost->state) {
        switch (ghost->state) {
        case GameState::Ghost::State::leavingGhostHouse:
            // We just go to the left after leaving the ghost house
            ghost->actor.currentDir = GameState::dirLeft;
            ghost->nextDir = ghost->actor.currentDir;
            break;

        case GameState::Ghost::State::enteringGhostHouse:
            // Ghosts that have been eaten can't be frightened again until another pill is eaten
            ghost->triggerFrightened.disable();
            break;

        case GameState::Ghost::State::scatter:
        case GameState::Ghost::State::chase:
            // Changing from scatter or chase mode makes the ghost reverse direction. Note that this does NOT happen when exiting frightened state
            ghost->nextDir = GameState::directionReverse(ghost->actor.currentDir);
            break;

        default:
            break;
        }
        ghost->state = ghostNextState;
    }
}

void PacmanGameModule::updateGameGhostTarget(PacmanGameModule::GameState::Ghost *ghost)
{
    assert(ghost != nullptr);

    switch (ghost->state) {
    case GameState::Ghost::State::scatter:
        // Each ghost heads to their corner of the maze
        assert(!(ghost->type < 0) && !(ghost->type >= GameState::ghostAmount));
        ghost->targetCell = GameState::ghostTargetPositionsScatter[ghost->type];
        break;

    case GameState::Ghost::State::chase:
        {
            const auto pacmanCellPosition = utils::posPixToCell(this->gameState.pacman.position, 8);
            const auto pacmanDirVec2u = GameState::directionToVector2u(this->gameState.pacman.currentDir);

            switch (ghost->type) {
            case GameState::ghostBlinky:
                // Blinky just goes straight after Pac-Man
                ghost->targetCell = pacmanCellPosition;
                break;

            case GameState::ghostPinky:
                // Pinky tries to go 4 cells in front of Pac-Man in whatever direction he is facing
                // There is a bug in the original game where when Pac-Man faces up, Pinky goes 4 cells up and 4 cells left instead of just 4 cells up. We replicate this here
                ghost->targetCell = utils::addVec2u(pacmanCellPosition, utils::mulVec2u(pacmanDirVec2u, 4));
                if (this->gameState.pacman.currentDir == GameState::dirUp)
                    ghost->targetCell.x -= 4;
                break;

            case GameState::ghostInky:
            {
                // Inky aims to go to the cell that is opposite to Blinky's position relative to the cell 2 cells in front of Pac-Man in whatever direction he is facing. Note that this is also affected by the overflow bug. Gotta say, Inky's movement is... quite unpredictable
                const auto blinkyCellPosition = utils::posPixToCell(this->gameState.ghosts[GameState::ghostBlinky].actor.position, 8);
                auto cellTwoInFrontOfPacMan = utils::addVec2u(pacmanCellPosition, utils::mulVec2u(pacmanDirVec2u, 2));
                if (this->gameState.pacman.currentDir == GameState::dirUp)
                    cellTwoInFrontOfPacMan.x -= 2;
                ghost->targetCell = utils::addVec2u(blinkyCellPosition, utils::mulVec2u(utils::subVec2u(cellTwoInFrontOfPacMan, blinkyCellPosition), 2));
                break;
            }

            case GameState::ghostClyde:
                // Clyde is the same as Blinky, except funnily enough the moment he gets close to Pac-Man he just starts running back to his scatter target
                if (utils::squareDistanceVec2u(utils::posPixToCell(ghost->actor.position, 8), pacmanCellPosition) > 64)
                    ghost->targetCell = pacmanCellPosition;
                else
                    ghost->targetCell = GameState::ghostTargetPositionsScatter[ghost->type];
                break;

            default:
                break;
            }
        }
        break;

    case GameState::Ghost::State::frightened:
        if (originalGameBehavior) {
            // A completely random target position is chosen in frightened mode
            ghost->targetCell = {rand() % PacmanGameModule::displayCells.x, rand() % PacmanGameModule::displayCells.y};
        } else {
            const auto pacmanCellPosition = utils::posPixToCell(this->gameState.pacman.position, 8);
            const auto ghostCellPosition = utils::posPixToCell(ghost->actor.position, 8);
            ghost->targetCell = utils::subVec2u(ghostCellPosition, utils::subVec2u(pacmanCellPosition, ghostCellPosition));
        }
        break;

    case GameState::Ghost::State::justEyes:
        // After having been eaten the target is the ghost house door
        ghost->targetCell = {13, 14};
        break;

    default:
        break;
    }
}

std::int32_t PacmanGameModule::updateGameGetRemainingDots()
{
    return (GameState::totalDotCount - this->gameState.dotsEatenCount);
}

unsigned PacmanGameModule::updateGameGetGhostMoveSpeed(const PacmanGameModule::GameState::Ghost *ghost)
{
    assert(ghost != nullptr);

    // Ghosts go at 50% speed in the ghost house, when leaving it and when frightened, go at 150% speed when they're just eyes (which is also the case graphically while entering the ghost house) and go much slower when in the tunnel
    // Otherwise, ghosts move very slightly slower than Pac-Man
    switch (ghost->state) {
    case GameState::Ghost::State::inGhostHouse:
    case GameState::Ghost::State::leavingGhostHouse:
    case GameState::Ghost::State::frightened:
        return this->currentFrame & 1;

    case GameState::Ghost::State::justEyes:
    case GameState::Ghost::State::enteringGhostHouse:
        return (this->currentFrame & 1) + 1;

    default:
        // Ghost speed depends on the current round (they go faster as rounds go on)... it's much slower when in a tunnel, though
        if (GameState::cellPositionIsInTunnel(utils::posPixToCell(ghost->actor.position, 8))) {
            if (this->gameState.currentRound == 0)
                return (this->currentFrame % 5) == 0 || (this->currentFrame % 5) == 2;
            return (this->currentFrame % 2) == 0;
        }
        else {
            if (ghost->type == GameState::ghostBlinky) {
                // Blinky becomes "Elroy" when Pac-Man has eaten enough dots, which makes him faster
                if (this->gameState.currentRound == 0) {
                    if (this->updateGameGetRemainingDots() < 10)
                        return bool(this->currentFrame % 6);
                    if (this->updateGameGetRemainingDots() < 20)
                        return bool(this->currentFrame % 5);
                } else if (this->gameState.currentRound >= 1 && this->gameState.currentRound <= 3) {
                    if (this->updateGameGetRemainingDots() < 15 + ((this->gameState.currentRound != 1) * 5))
                        return bool(this->currentFrame % 20);
                    if (this->updateGameGetRemainingDots() < 30 + ((this->gameState.currentRound != 1) * 10))
                        return bool(this->currentFrame % 10);
                } else {
                    if (this->updateGameGetRemainingDots() < 20
                        + ((this->gameState.currentRound > 4) * 5)
                        + ((this->gameState.currentRound > 7) * 5)
                        + ((this->gameState.currentRound > 10) * 10)
                        + ((this->gameState.currentRound > 13) * 10)
                        + ((this->gameState.currentRound > 17) * 10))
                        return 1 + !bool(this->currentFrame % 20);
                    if (this->updateGameGetRemainingDots() < 40
                        + ((this->gameState.currentRound > 4) * 10)
                        + ((this->gameState.currentRound > 7) * 10)
                        + ((this->gameState.currentRound > 10) * 20)
                        + ((this->gameState.currentRound > 13) * 20)
                        + ((this->gameState.currentRound > 17) * 20))
                        return 1;
                }
            }

            if (this->gameState.currentRound == 0)
                return bool(this->currentFrame % 4);
            if (this->gameState.currentRound >= 1 && this->gameState.currentRound <= 3)
                return bool(this->currentFrame % 6);
            return bool(this->currentFrame % 20);
        }
    }
}

bool PacmanGameModule::updateGameGhostDirection(PacmanGameModule::GameState::Ghost *ghost)
{
    assert(ghost != nullptr);

    switch (ghost->state) {
    case GameState::Ghost::State::inGhostHouse:
        // If the ghost is in the ghost house, they go up and down
        if (ghost->actor.position.y <= 17 * 8)
            ghost->nextDir = GameState::dirDown;
        if (ghost->actor.position.y >= 18 * 8)
            ghost->nextDir = GameState::dirUp;
        ghost->actor.currentDir = ghost->nextDir;
        return true;

    case GameState::Ghost::State::leavingGhostHouse:
        // If the ghost is leaving the ghost house, we need to get them out of there
        if (ghost->actor.position.x == GameState::ghostHousePixelEnterPoint.x) {
            if (ghost->actor.position.y > GameState::ghostHousePixelEnterPoint.y)
                ghost->nextDir = GameState::dirUp;
        }
        else {
            const std::int32_t middleY = 17 * 8 + 4;
            if (ghost->actor.position.y > middleY)
                ghost->nextDir = GameState::dirUp;
            else if (ghost->actor.position.y < middleY)
                ghost->nextDir = GameState::dirDown;
            else
                ghost->nextDir = (ghost->actor.position.x > GameState::ghostHousePixelEnterPoint.x) ? GameState::dirLeft : GameState::dirRight;
        }
        ghost->actor.currentDir = ghost->nextDir;
        return true;

    case GameState::Ghost::State::enteringGhostHouse:
    {
        // If the ghost is entering the ghost house, we just navigate towards the target position in the ghost house
        const auto ghostCellPosition = utils::posPixToCell(ghost->actor.position, 8);
        const auto targetPixelPosition = GameState::ghostTargetPositionsInGhostHouse[ghost->type];

        if (ghostCellPosition.y == 14) {
            if (ghost->actor.position.x != GameState::ghostHousePixelEnterPoint.x)
                ghost->nextDir = (ghost->actor.position.x < GameState::ghostHousePixelEnterPoint.x) ? GameState::dirRight : GameState::dirLeft;
            else
                ghost->nextDir = GameState::dirDown;
        } else if (ghost->actor.position.y == targetPixelPosition.y)
            ghost->nextDir = (ghost->actor.position.x < targetPixelPosition.x) ? GameState::dirRight : GameState::dirLeft;
        ghost->actor.currentDir = ghost->nextDir;
        return true;
    }

    default:
        // Otherwise, we just go towards the targeted cell
        // Note that we only change direction when we're at the midpoint of a cell
        const auto distanceToCellMiddle = this->distanceToCellMiddle(ghost->actor.position);
        if ((distanceToCellMiddle.x == 0) && (distanceToCellMiddle.y == 0)) {
            ghost->actor.currentDir = ghost->nextDir;

            const auto lookAheadPosition = utils::addVec2u(utils::posPixToCell(ghost->actor.position, 8), GameState::directionToVector2u(ghost->actor.currentDir));
            unsigned minimumDistance = 100000;
            unsigned distance = 0;
            for (unsigned currentDir = 0; currentDir < GameState::dirCount; ++currentDir) {
                // The red zone of the maze makes it so that ghosts can't move up in it, which is easily exploitable
                if (GameState::cellPositionIsInRedzone(lookAheadPosition) && (currentDir == GameState::dirUp) && (ghost->state != GameState::Ghost::State::justEyes))
                    continue;

                const auto testPosition = PacmanGameModule::clampCellPosition(utils::addVec2u(lookAheadPosition, GameState::directionToVector2u(static_cast<GameState::Direction>(currentDir))));
                if ((GameState::directionReverse(static_cast<GameState::Direction>(currentDir)) != ghost->actor.currentDir) &&
                    !this->updateGameIsCellBlocking(testPosition)) {
                    distance = utils::squareDistanceVec2u(testPosition, ghost->targetCell);
                    if (distance < minimumDistance) {
                        minimumDistance = distance;
                        ghost->nextDir = static_cast<GameState::Direction>(currentDir);
                    }
                }
            }
        }
        return false;
    }
}

void PacmanGameModule::updateGameCells()
{
    this->setCellScore({6, 1}, this->gameState.score);
    if (this->gameState.highScore != 0)
        this->setCellScore({16, 1}, this->gameState.highScore);

    // The pills blink whene in-game
    constexpr ICore::Vector2u pillPositions[] = {
        {1, 6},
        {26, 6},
        {1, 26},
        {26, 26},
    };
    for (auto i : pillPositions)
        if (this->gameState.freezeReason & (GameState::freezeReasonPrelude | GameState::freezeReasonReady))
            this->setCellEnable(i, true);
        else
            this->setCellEnable(i, this->currentFrame & 0x10);

    // We remove the bonus fruit score after a while
    if (this->gameState.triggerAteBonusFruit.isExactlyAfter(this, 2 * 60))
        this->setCellScoreFruit(PacmanGameModule::GameState::BonusFruit::none);

    // We display the amount of remaining lives at the bottom left
    for (std::int32_t i = 0; i < GameState::initialLifeCount; ++i) {
        this->setCellCharacter({2 + 2 * static_cast<std::uint32_t>(i), 34}, ' ');
        if (i < this->gameState.currentLives)
            this->setCellSprite({2 + 2 * static_cast<std::uint32_t>(i), 34}, this->texturePacmanFaceLeft1);
    }

    // We display the fruit list in the bottom right corner
    {
        std::uint32_t cellX = 24;
        for (std::int32_t i = (this->gameState.currentRound - 7 + 1); i <= static_cast<std::int32_t>(this->gameState.currentRound); ++i)
            if (!(i < 0)) {
                this->setCellSprite({cellX, 34}, this->getBonusFruitTexture(GameState::getLevelInformation(i).bonusFruit));
                cellX -= 2;
            }
    }

    // TODO: Make the maze blink between blue and white when the round has been won
}

void PacmanGameModule::updateGameActors()
{
    if (this->updateGameShouldPacmanMove()) {
        const PacmanGameModule::GameState::Direction wantedDirection = this->currentInputDirection;

        // We look ahead to check if the direction we want is blocked
        if (this->updateGameCanMoveTo(this->gameState.pacman.position, wantedDirection, true))
            this->gameState.pacman.currentDir = wantedDirection;

        // Move into the selected direction
        if (this->updateGameCanMoveTo(this->gameState.pacman.position, this->gameState.pacman.currentDir, true)) {
            this->gameState.pacman.position = this->updateGameMove(this->gameState.pacman.position, this->gameState.pacman.currentDir, true);
            ++this->gameState.pacman.animationFrame;
        }

        // Check if we should eat a dot or an energizer pill
        const auto pacmanCellPosition = utils::posPixToCell(this->gameState.pacman.position, 8);
        if (this->updateGameIsCellDot(pacmanCellPosition)) {
            this->gameState.triggerAteDot.setNow(this);
            this->gameState.triggerHouseLeaveForce.setNow(this);
            this->setCellCharacter(pacmanCellPosition, ' ', ICore::Color::black);
            ++this->gameState.score;
            this->updateGameDotEaten();
            this->updateGameGhosthouseDotCounters();
        }
        if (this->updateGameIsCellPill(pacmanCellPosition)) {
            this->gameState.triggerAtePill.setNow(this);
            this->setCellCharacter(pacmanCellPosition, ' ', ICore::Color::black);
            this->gameState.score += 5;
            this->updateGameDotEaten();
            this->gameState.ghostsEatenCount = 0;
            for (auto &i : this->gameState.ghosts)
                i.triggerFrightened.setNow(this);
            this->soundHandler.startSound("assets/pacman/power-pellet.wav", 1, true);
        }

        // Check if we should eat the bonus fruit
        if (this->gameState.activeBonusFruit != PacmanGameModule::GameState::BonusFruit::none) {
            const auto checkCellPosition = utils::posPixToCell(utils::addVec2u(this->gameState.pacman.position, {8 / 2, 0}), 8);
            if (utils::eqVec2u(checkCellPosition, {14, 20})) {
                this->gameState.triggerAteBonusFruit.setNow(this);
                this->gameState.score += PacmanGameModule::GameState::getLevelInformation(this->gameState.currentRound).bonusFruitScore;
                this->setCellScoreFruit(this->gameState.activeBonusFruit);
                this->gameState.activeBonusFruit = PacmanGameModule::GameState::BonusFruit::none;
                this->soundHandler.startSound("assets/pacman/eat-fruit.wav", 2);
            }
        }

        // Check if we're touching any of the ghosts
        for (auto &currentGhost : this->gameState.ghosts) {
            const auto ghostCellPosition = utils::posPixToCell(currentGhost.actor.position, 8);
            if (utils::eqVec2u(ghostCellPosition, pacmanCellPosition)) {
                if (currentGhost.state == PacmanGameModule::GameState::Ghost::State::frightened) {
                    currentGhost.state = PacmanGameModule::GameState::Ghost::State::justEyes;
                    currentGhost.triggerGotEaten.setNow(this);
                    this->gameState.triggerAteGhost.setNow(this);
                    ++this->gameState.ghostsEatenCount;
                    this->gameState.score += 10 * (1 << this->gameState.ghostsEatenCount);
                    this->gameState.freezeReason |= PacmanGameModule::GameState::freezeReasonAteGhost;
                    if (debugFreeze)
                        puts("freeze ate ghost");
                    this->soundHandler.startSound("assets/pacman/eat-ghost.wav", 2);
                } else if ((currentGhost.state == PacmanGameModule::GameState::Ghost::State::chase) ||
                           (currentGhost.state == PacmanGameModule::GameState::Ghost::State::scatter)) {
                    if (!debugGodMode) {
                        this->soundHandler.stopAllSounds();
                        this->gameState.triggerPacmanKilled.setNow(this);
                        this->gameState.freezeReason |= PacmanGameModule::GameState::freezeReasonPacmanKilled;
                        if (debugFreeze)
                            puts("freeze pacman died");

                        // Either:
                        // Pac-Man has some lives left: We thus start a new round
                        // Pac-Man has no lives left: We thus start the game over sequence
                        if (this->gameState.currentLives > 0)
                            this->gameState.triggerReady.setAfter(this, PacmanGameModule::GameState::freezeFramesAfterPacmanKilled + PacmanGameModule::GameState::freezeFramesPacmanDeathSequence);
                        else
                            this->gameState.triggerGameOver.setAfter(this, PacmanGameModule::GameState::freezeFramesAfterPacmanKilled + PacmanGameModule::GameState::freezeFramesPacmanDeathSequence);
                    }
                }
            }
        }
    }

    // Run all of the AIs for the ghosts
    for (auto &currentGhost : this->gameState.ghosts) {
        this->updateGameGhostState(&currentGhost);
        this->updateGameGhostTarget(&currentGhost);

        const unsigned moveSpeed = this->updateGameGetGhostMoveSpeed(&currentGhost);
        for (unsigned i = 0; i < moveSpeed; ++i) {
            bool forcedMove = this->updateGameGhostDirection(&currentGhost);
            if (forcedMove || this->updateGameCanMoveTo(currentGhost.actor.position, currentGhost.actor.currentDir, false)) {
                currentGhost.actor.position = this->updateGameMove(currentGhost.actor.position, currentGhost.actor.currentDir, false);
                ++currentGhost.actor.animationFrame;
            }
        }
    }
}

void PacmanGameModule::updateGameSpritePacmanDeath(std::uint64_t frame)
{
    std::size_t textureId = (frame / 8);
    if (textureId > 11)
        textureId = 11;

    constexpr ICore::Texture *PacmanGameModule::* textures[12] = {
        &PacmanGameModule::texturePacmanDeath1,
        &PacmanGameModule::texturePacmanDeath1,
        &PacmanGameModule::texturePacmanDeath2,
        &PacmanGameModule::texturePacmanDeath3,
        &PacmanGameModule::texturePacmanDeath4,
        &PacmanGameModule::texturePacmanDeath5,
        &PacmanGameModule::texturePacmanDeath6,
        &PacmanGameModule::texturePacmanDeath7,
        &PacmanGameModule::texturePacmanDeath8,
        &PacmanGameModule::texturePacmanDeath9,
        &PacmanGameModule::texturePacmanDeath10,
        &PacmanGameModule::texturePacmanDeath11
    };
    this->spritePacman->texture = this->*textures[textureId];
}

void PacmanGameModule::updateGameSpritePacman(GameState::Direction direction, std::uint64_t frame)
{
    constexpr ICore::Texture *PacmanGameModule::* textures[4][4] = {
        {&PacmanGameModule::texturePacmanFaceClosed, &PacmanGameModule::texturePacmanFaceRight1, &PacmanGameModule::texturePacmanFaceRight2, &PacmanGameModule::texturePacmanFaceRight1},
        {&PacmanGameModule::texturePacmanFaceClosed, &PacmanGameModule::texturePacmanFaceDown1, &PacmanGameModule::texturePacmanFaceDown2, &PacmanGameModule::texturePacmanFaceDown1},
        {&PacmanGameModule::texturePacmanFaceClosed, &PacmanGameModule::texturePacmanFaceLeft1, &PacmanGameModule::texturePacmanFaceLeft2, &PacmanGameModule::texturePacmanFaceLeft1},
        {&PacmanGameModule::texturePacmanFaceClosed, &PacmanGameModule::texturePacmanFaceUp1, &PacmanGameModule::texturePacmanFaceUp2, &PacmanGameModule::texturePacmanFaceUp1},
    };
    std::size_t textureId = (frame / 4) & 3;
    this->spritePacman->texture = this->*textures[direction][textureId];
}

void PacmanGameModule::updateGameSpriteGhostJustEyes(GameState::GhostType ghostType, GameState::Direction direction)
{
    constexpr ICore::Texture *PacmanGameModule::* textures[4] = {
        &PacmanGameModule::textureGhostEyesRight,
        &PacmanGameModule::textureGhostEyesDown,
        &PacmanGameModule::textureGhostEyesLeft,
        &PacmanGameModule::textureGhostEyesUp,
    };

    assert(!(direction < 0) && !(direction >= GameState::dirCount));
    this->sprites[1 + ghostType].texture = this->*textures[direction];
}

void PacmanGameModule::updateGameSpriteGhostFrightened(GameState::GhostType ghostType, std::uint64_t frame)
{
    constexpr ICore::Texture *PacmanGameModule::* textures[2][2] = {
        {&PacmanGameModule::textureGhostFrightenedBlue1, &PacmanGameModule::textureGhostFrightenedBlue2},
        {&PacmanGameModule::textureGhostFrightenedWhite1, &PacmanGameModule::textureGhostFrightenedWhite2},
    };

    std::size_t textureId = (frame / 4) & 1;
    if (GameState::getLevelInformation(this->gameState.currentRound).afraidFrames - 60 < static_cast<std::int64_t>(frame))
        this->sprites[1 + ghostType].texture = this->*textures[!(frame & 0x10)][textureId];
    else
        this->sprites[1 + ghostType].texture = this->*textures[0][textureId];
}

void PacmanGameModule::updateGameSpriteGhost(GameState::GhostType ghostType, GameState::Direction direction, std::uint64_t frame)
{
    constexpr ICore::Texture *PacmanGameModule::* textures[4][4][2] = {
        {
            {&PacmanGameModule::textureBlinkyFaceRight1, &PacmanGameModule::textureBlinkyFaceRight2},
            {&PacmanGameModule::textureBlinkyFaceDown1, &PacmanGameModule::textureBlinkyFaceDown2},
            {&PacmanGameModule::textureBlinkyFaceLeft1, &PacmanGameModule::textureBlinkyFaceLeft2},
            {&PacmanGameModule::textureBlinkyFaceUp1, &PacmanGameModule::textureBlinkyFaceUp2},
        },
        {
            {&PacmanGameModule::texturePinkyFaceRight1, &PacmanGameModule::texturePinkyFaceRight2},
            {&PacmanGameModule::texturePinkyFaceDown1, &PacmanGameModule::texturePinkyFaceDown2},
            {&PacmanGameModule::texturePinkyFaceLeft1, &PacmanGameModule::texturePinkyFaceLeft2},
            {&PacmanGameModule::texturePinkyFaceUp1, &PacmanGameModule::texturePinkyFaceUp2},
        },
        {
            {&PacmanGameModule::textureInkyFaceRight1, &PacmanGameModule::textureInkyFaceRight2},
            {&PacmanGameModule::textureInkyFaceDown1, &PacmanGameModule::textureInkyFaceDown2},
            {&PacmanGameModule::textureInkyFaceLeft1, &PacmanGameModule::textureInkyFaceLeft2},
            {&PacmanGameModule::textureInkyFaceUp1, &PacmanGameModule::textureInkyFaceUp2},
        },
        {
            {&PacmanGameModule::textureClydeFaceRight1, &PacmanGameModule::textureClydeFaceRight2},
            {&PacmanGameModule::textureClydeFaceDown1, &PacmanGameModule::textureClydeFaceDown2},
            {&PacmanGameModule::textureClydeFaceLeft1, &PacmanGameModule::textureClydeFaceLeft2},
            {&PacmanGameModule::textureClydeFaceUp1, &PacmanGameModule::textureClydeFaceUp2},
        },
    };

    std::size_t textureId = (frame / 8) & 1;
    assert(!(direction < 0) && !(direction >= GameState::dirCount));
    assert(!(ghostType < 0) && !(ghostType >= GameState::ghostAmount));
    this->sprites[1 + ghostType].texture = this->*textures[ghostType][direction][textureId];
}

void PacmanGameModule::updateGameSprites()
{
    if (this->spritePacman->enabled) {
        this->spritePacman->position = PacmanGameModule::actorPositionToSpritePosition(this->gameState.pacman.position);
        if (this->gameState.freezeReason & GameState::freezeReasonAteGhost)
            this->spritePacman->texture = nullptr;
        else if (this->gameState.freezeReason & (GameState::freezeReasonPrelude | GameState::freezeReasonReady))
            this->spritePacman->texture = this->texturePacmanFaceClosed;
        else if (this->gameState.freezeReason & (GameState::freezeReasonPacmanKilled)) {
            if (this->gameState.triggerPacmanKilled.isAfter(this, GameState::freezeFramesAfterPacmanKilled))
                this->updateGameSpritePacmanDeath(this->gameState.triggerPacmanKilled.framesSince(this) - GameState::freezeFramesAfterPacmanKilled);
        } else
            this->updateGameSpritePacman(this->gameState.pacman.currentDir, this->gameState.pacman.animationFrame);
    }
    for (unsigned i = 0; i < std::size(this->gameState.ghosts); ++i) {
        auto &sprite = this->sprites[1 + i];
        if (sprite.enabled) {
            sprite.position = PacmanGameModule::actorPositionToSpritePosition(this->gameState.ghosts[i].actor.position);
            if (this->gameState.freezeReason & GameState::freezeReasonPacmanKilled) {
                if (this->gameState.triggerPacmanKilled.isAfter(this, GameState::freezeFramesAfterPacmanKilled))
                    sprite.texture = nullptr;
            } else if (this->gameState.freezeReason & GameState::freezeReasonWon)
                sprite.texture = nullptr;
            else
                switch (this->gameState.ghosts[i].state) {
                case GameState::Ghost::State::justEyes:
                    if (this->gameState.ghosts[i].triggerGotEaten.isBefore(this, GameState::freezeFramesAfterGhostEaten)) {
                        // TODO: render score instead of ghost when it gets eaten
                        sprite.texture = nullptr;
                    } else
                        this->updateGameSpriteGhostJustEyes(static_cast<GameState::GhostType>(i), this->gameState.ghosts[i].nextDir);
                    break;

                case GameState::Ghost::State::enteringGhostHouse:
                    this->updateGameSpriteGhostJustEyes(static_cast<GameState::GhostType>(i), this->gameState.ghosts[i].nextDir);
                    break;

                case GameState::Ghost::State::frightened:
                    this->updateGameSpriteGhostFrightened(static_cast<GameState::GhostType>(i), this->gameState.ghosts[i].triggerFrightened.framesSince(this));
                    break;

                default:
                    this->updateGameSpriteGhost(static_cast<GameState::GhostType>(i), this->gameState.ghosts[i].nextDir, this->gameState.ghosts[i].actor.animationFrame);
                    break;
                }
        }
    }

    if (this->gameState.activeBonusFruit == GameState::BonusFruit::none)
        this->spriteFruit->enabled = false;
    else {
        this->spriteFruit->enabled = true;
        this->spriteFruit->position = {13 * 8, 19 * 8 + 4};
        this->spriteFruit->texture = this->getBonusFruitTexture(this->gameState.activeBonusFruit);
    }
}

void PacmanGameModule::updateGame()
{
    // Make sure we only initialize the game mode's state once
    if (this->gameState.triggerStart.isNow(this)) {
        this->gameState.triggerReady.setAfter(this, 120);
        this->soundHandler.startSound("assets/pacman/game-start.wav", 0);
        this->updateGameInit();
    }

    // Start a new round
    if (this->gameState.triggerReady.isNow(this)) {
        this->updateGameInitRound();
        this->gameState.triggerRoundStarted.setAfter(this, 2 * 60 + 10);
    }
    if (this->gameState.triggerRoundStarted.isNow(this)) {
        this->gameState.freezeReason &= ~PacmanGameModule::GameState::freezeReasonReady;
        if (debugFreeze)
            puts("freeze remove ready");

        // This removes the "READY!" text
        this->setCellText({11, 20}, "      ");
        this->soundHandler.startSound("assets/pacman/siren-1.wav", 1, true);
    }

    // This activates the bonus fruit
    if (this->gameState.triggerActivateBonusFruit.isNow(this))
        this->gameState.activeBonusFruit = GameState::getLevelInformation(this->gameState.currentRound).bonusFruit;
    if (this->gameState.triggerActivateBonusFruit.isExactlyAfter(this, 10 * 60)) // Bonus fruits disappear after 10 seconds
        this->gameState.activeBonusFruit = GameState::BonusFruit::none;

    // If we're finishing up the frightened state, play the siren again
    if (this->gameState.triggerAtePill.isExactlyAfter(this, GameState::getLevelInformation(this->gameState.currentLives).afraidFrames) &&
        !(this->gameState.freezeReason & (GameState::freezeReasonWon | GameState::freezeReasonReady)))
        this->soundHandler.startSound("assets/pacman/siren-1.wav", 1, true);

    // We end the frozen state after a while if it was started because Pac-Man ate a ghost
    if (this->gameState.freezeReason & GameState::freezeReasonAteGhost &&
        this->gameState.triggerAteGhost.isExactlyAfter(this, GameState::freezeFramesAfterGhostEaten)) {
        this->gameState.freezeReason &= ~GameState::freezeReasonAteGhost;
        if (debugFreeze)
            puts("freeze remove ate ghost");
    }

    // Play the sound of Pac-Man being dead if we need to
    if (this->gameState.triggerPacmanKilled.isExactlyAfter(this, GameState::freezeFramesAfterPacmanKilled))
        this->soundHandler.startSound("assets/pacman/death-1.wav", 2);
    if (this->gameState.triggerPacmanKilled.isExactlyAfter(this, GameState::freezeFramesAfterPacmanKilled + 75))
        this->soundHandler.startSound("assets/pacman/death-2.wav", 2);
    if (this->gameState.triggerPacmanKilled.isExactlyAfter(this, GameState::freezeFramesAfterPacmanKilled + 86))
        this->soundHandler.startSound("assets/pacman/death-2.wav", 2);

    if (this->gameState.freezeReason == 0)
        this->updateGameActors();
    this->updateGameCells();
    this->updateGameSprites();

    this->gameState.highScore = std::max(this->gameState.highScore, this->gameState.score);

    if (this->gameState.triggerRoundWon.isNow(this)) {
        this->gameState.freezeReason |= GameState::freezeReasonWon;
        if (debugFreeze)
            puts("freeze won");
        this->gameState.triggerReady.setAfter(this, 4 * 60);
    }
    if (this->gameState.triggerGameOver.isNow(this)) {
        this->setCellText({9, 20}, "GAME  OVER", ICore::Color::red);
        this->inputEnabled = false;
        this->introState.triggerStart.setAfter(this, 3 * 60);
    }
}

void PacmanGameModule::updateCurrentInputDirection()
{
    if (this->isButtonPressed(ICore::Button::Up) || this->isButtonPressed(ICore::Button::Y))
        this->currentInputDirection = PacmanGameModule::GameState::dirUp;
    if (this->isButtonPressed(ICore::Button::Down) || this->isButtonPressed(ICore::Button::A))
        this->currentInputDirection = PacmanGameModule::GameState::dirDown;
    if (this->isButtonPressed(ICore::Button::Right) || this->isButtonPressed(ICore::Button::B))
        this->currentInputDirection = PacmanGameModule::GameState::dirRight;
    if (this->isButtonPressed(ICore::Button::Left) || this->isButtonPressed(ICore::Button::X))
        this->currentInputDirection = PacmanGameModule::GameState::dirLeft;
}

void PacmanGameModule::update()
{
    this->updateCurrentInputDirection();

    ++this->currentFrame;
    if (this->introState.triggerStart.isNow(this))
        this->mode = PacmanGameModule::Mode::intro;
    if (this->gameState.triggerStart.isNow(this))
        this->mode = PacmanGameModule::Mode::game;
    switch (this->mode) {
    case PacmanGameModule::Mode::intro:
        this->updateIntro();
        break;
    case PacmanGameModule::Mode::game:
        this->updateGame();
        break;
    }
}

void PacmanGameModule::draw()
{
    this->coreHandle->clearScreen(ICore::Color::black);
    for (const auto &i : this->drawnCellThings) {
        if (i.second.enabled) {
            if (std::holds_alternative<DrawableCellCharacter>(i.second.thing)) {
                auto cellCharacter = std::get<DrawableCellCharacter>(i.second.thing);
                this->textHandler.drawText(std::string(1, cellCharacter.character), std::numeric_limits<std::size_t>::max(), utils::posCellToPix(i.first, 8), cellCharacter.color);
            }
            else if (std::holds_alternative<DrawableCellTexture>(i.second.thing)) {
                auto cellTexture = std::get<DrawableCellTexture>(i.second.thing);
                auto realPosition = utils::posCellToPix(i.first, 8);
                realPosition.x += cellTexture.realPositionOffset.x;
                realPosition.y += cellTexture.realPositionOffset.y;
                assert(cellTexture.texture != nullptr);
                this->coreHandle->renderSprite({realPosition, cellTexture.texture});
            }
            else
                assert(false && "wtf ???");
        }
    }
    for (const auto &i : this->sprites) {
        if (i.enabled && i.texture != nullptr)
            this->coreHandle->renderSprite({i.position, i.texture});
    }

    // Debug code to display every ghost's target
    if (debugTargets) {
        this->textHandler.drawText(".", 1, utils::posCellToPix(this->gameState.blinky->targetCell, 8), ICore::Color::red);
        this->textHandler.drawText(".", 1, utils::posCellToPix(this->gameState.pinky->targetCell, 8), ICore::Color::magenta);
        this->textHandler.drawText(".", 1, utils::posCellToPix(this->gameState.inky->targetCell, 8), ICore::Color::cyan);
        this->textHandler.drawText(".", 1, utils::posCellToPix(this->gameState.clyde->targetCell, 8), ICore::Color::yellow);
    }
}
