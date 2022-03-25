#include "pacmanGameModule.hpp"
#include "utils/positionConverters.hpp"
#include <limits>
#include <cassert>

void PacmanGameModule::init(ICore *coreHandle)
{
    this->coreHandle = coreHandle;

    this->coreHandle->setPixelsPerCell(8);
    this->coreHandle->openWindow({224, 288});
    this->textHandler.init(this->coreHandle, 8, "assets/font/emulogic.ttf");
    this->textureBlinkyFaceRight1 = this->coreHandle->loadTexture("assets/pacman/blinky-face-right-1.png", 'R', ICore::Color::red, ICore::Color::none, 12, 12);
    this->texturePinkyFaceRight1 = this->coreHandle->loadTexture("assets/pacman/pinky-face-right-1.png", 'R', ICore::Color::magenta, ICore::Color::none, 12, 12);
    this->textureInkyFaceRight1 = this->coreHandle->loadTexture("assets/pacman/inky-face-right-1.png", 'R', ICore::Color::cyan, ICore::Color::none, 12, 12);
    this->textureClydeFaceRight1 = this->coreHandle->loadTexture("assets/pacman/clyde-face-right-1.png", 'R', ICore::Color::yellow, ICore::Color::none, 12, 12);
    this->textureDot = this->coreHandle->loadTexture("assets/pacman/dot.png", '.', ICore::Color::white, ICore::Color::none, 8, 8);
    this->texturePill = this->coreHandle->loadTexture("assets/pacman/pill.png", '*', ICore::Color::white, ICore::Color::none, 8, 8);

    this->startIntroAction.start(this);
}

void PacmanGameModule::resetAllDraw()
{
    this->drawnCellThings.clear();
}

bool PacmanGameModule::isValidCell(ICore::Vector2u cellPosition)
{
    return (cellPosition.x < this->displayCells.x && cellPosition.y < this->displayCells.y);
}

void PacmanGameModule::setCellCharacter(ICore::Vector2u cellPosition, char character, ICore::Color color)
{
    assert(this->isValidCell(cellPosition));
    this->drawnCellThings[cellPosition] = DrawableCellCharacter{character, color};
}

void PacmanGameModule::setCellSprite(ICore::Vector2u cellPosition, ICore::Texture *texture, ICore::Vector2u realPositionOffset)
{
    assert(this->isValidCell(cellPosition));
    this->drawnCellThings[cellPosition] = DrawableCellTexture{texture, realPositionOffset};
}

void PacmanGameModule::setCellText(ICore::Vector2u cellPosition, std::string_view text, ICore::Color color)
{
    assert(this->isValidCell(cellPosition));

    for (auto i : text) {
        if (cellPosition.x < this->displayCells.x) {
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

void PacmanGameModule::updateIntro()
{
    // When we start up the intro, draw the initial text
    if (this->startIntroAction.isNow(this)) {
        this->resetAllDraw();
        this->setCellText({3, 0}, "1UP   HIGH SCORE   2UP");
        this->setCellScore({6, 1}, 0);
        this->setCellText({7, 5}, "CHARACTER / NICKNAME");
        this->setCellText({3, 35}, "CREDIT  0");
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
        if (this->startIntroAction.isExactlyAfter(this, totalDelay))
            this->setCellSprite({4, yPos}, this->*ghostsInfo[i].texture, {std::uint32_t(-4), std::uint32_t(-4)});

        // After 1 more second, the ghost's nickname appears
        totalDelay += 60;
        if (this->startIntroAction.isExactlyAfter(this, totalDelay))
            this->setCellText({7, yPos}, ghostsInfo[i].nickname, ghostsInfo[i].color);

        // After 0.5 more seconds, the ghost's name appears
        totalDelay += 30;
        if (this->startIntroAction.isExactlyAfter(this, totalDelay))
            this->setCellText({17, yPos}, ghostsInfo[i].name, ghostsInfo[i].color);
    }

    // After 1 more second, the dots score info stuff appears
    totalDelay += 60;
    if (this->startIntroAction.isExactlyAfter(this, totalDelay)) {
        this->setCellSprite({10, 24}, this->textureDot);
        this->setCellSprite({10, 26}, this->texturePill);
        this->setCellText({12, 24}, "10 pts");
        this->setCellText({12, 26}, "50 pts");
    }

    // After 1 more second, we start displaying "PRESS ANY KEY TO START!"
    totalDelay += 60;
    if (this->startIntroAction.isAfter(this, totalDelay)) {
        if ((this->startIntroAction.framesSince(this) % 60) > 30)
            this->setCellText({1, 31}, "                          ");
        else
            this->setCellText({1, 31}, "PRESS ANY BUTTON TO START!");
    }

    // Fuck it I'm not doing the chase sequence

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
        if (this->coreHandle->isButtonPressed(i)) {
            this->startGameAction.start(this);
        }
}

void PacmanGameModule::update()
{
    ++this->currentFrame;
    if (this->startIntroAction.isNow(this))
        this->mode = PacmanGameModule::Mode::intro;
    if (this->startGameAction.isNow(this))
        this->mode = PacmanGameModule::Mode::game;
    switch (this->mode) {
    case PacmanGameModule::Mode::intro:
        this->updateIntro();
        break;
    case PacmanGameModule::Mode::game:
        throw std::runtime_error("TODO");
        //this->updateGame();
        break;
    }
}

void PacmanGameModule::draw()
{
    this->coreHandle->clearScreen(ICore::Color::black);
    for (auto &i : this->drawnCellThings) {
        if (std::holds_alternative<DrawableCellCharacter>(i.second)) {
            auto cellCharacter = std::get<DrawableCellCharacter>(i.second);
            this->textHandler.drawText(std::string(1, cellCharacter.character), std::numeric_limits<std::size_t>::max(), utils::posCellToPix(i.first, 8), cellCharacter.color);
        }
        else if (std::holds_alternative<DrawableCellTexture>(i.second)) {
            auto cellTexture = std::get<DrawableCellTexture>(i.second);
            auto realPosition = utils::posCellToPix(i.first, 8);
            realPosition.x += cellTexture.realPositionOffset.x;
            realPosition.y += cellTexture.realPositionOffset.y;
            this->coreHandle->renderSprite({realPosition, cellTexture.texture});
        }
        else
            assert(false && "wtf ???");
    }
}
