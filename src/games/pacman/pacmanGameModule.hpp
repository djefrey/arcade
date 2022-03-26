#pragma once

#include "arcade-interface/IGameModule.hpp"
#include "utils/textHandler.hpp"
#include "utils/vector2u.hpp"
#include "utils/soundHandler.hpp"
#include <vector>
#include <variant>
#include <optional>
#include <limits>

class PacmanGameModule : public virtual IGameModule {
    ICore *coreHandle = nullptr;
    utils::TextHandler textHandler;
    utils::SoundHandler soundHandler;

    ICore::Texture *textureBlinkyFaceRight1;
    ICore::Texture *texturePinkyFaceRight1;
    ICore::Texture *textureInkyFaceRight1;
    ICore::Texture *textureClydeFaceRight1;
    ICore::Texture *textureDot;
    ICore::Texture *texturePill;
    ICore::Texture *texturePts;
    ICore::Texture *textureMazeBoxBottomLeft;
    ICore::Texture *textureMazeBoxBottom;
    ICore::Texture *textureMazeBoxBottomRight;
    ICore::Texture *textureMazeBoxLeft;
    ICore::Texture *textureMazeBoxRight;
    ICore::Texture *textureMazeBoxTopLeft;
    ICore::Texture *textureMazeBoxTop;
    ICore::Texture *textureMazeBoxTopRight;
    ICore::Texture *textureMazeLeft;
    ICore::Texture *textureMazeRight;
    ICore::Texture *textureMazeTopLeft;
    ICore::Texture *textureMazeTopMiddleLeft;
    ICore::Texture *textureMazeTopMiddleRight;
    ICore::Texture *textureMazeTop;
    ICore::Texture *textureMazeTopRight;
    ICore::Texture *textureMazeBoxBottomToLeftCorner;
    ICore::Texture *textureMazeBoxBottomToRightCorner;
    ICore::Texture *textureMazeBoxTopToRightCorner;
    ICore::Texture *textureMazeBoxTopToLeftCorner;
    ICore::Texture *textureMazeMiddleBoxTopLeft;
    ICore::Texture *textureMazeMiddleBoxTop;
    ICore::Texture *textureMazeMiddleBoxEntranceLeft;
    ICore::Texture *textureMazeMiddleBoxEntranceRight;
    ICore::Texture *textureMazeMiddleBoxEntranceMiddle;
    ICore::Texture *textureMazeMiddleBoxTopRight;
    ICore::Texture *textureMazeMiddleBoxLeft;
    ICore::Texture *textureMazeMiddleBoxRight;
    ICore::Texture *textureMazeMiddleBoxBottom;
    ICore::Texture *textureMazeMiddleBoxBottomLeft;
    ICore::Texture *textureMazeMiddleBoxBottomRight;
    ICore::Texture *textureMazeBottom;
    ICore::Texture *textureMazeRightMiddleTop;
    ICore::Texture *textureMazeRightMiddleBottom;
    ICore::Texture *textureMazeLeftMiddleTop;
    ICore::Texture *textureMazeLeftMiddleBottom;
    ICore::Texture *textureMazeBottomLeft;
    ICore::Texture *textureMazeBottomRight;
    ICore::Texture *textureFruitApple;
    ICore::Texture *textureFruitBell;
    ICore::Texture *textureFruitCherries;
    ICore::Texture *textureFruitGalaxian;
    ICore::Texture *textureFruitGrapes;
    ICore::Texture *textureFruitKey;
    ICore::Texture *textureFruitPeach;
    ICore::Texture *textureFruitStrawberry;
    ICore::Texture *texturePacmanFaceClosed;
    ICore::Texture *texturePacmanDeath1;
    ICore::Texture *texturePacmanDeath2;
    ICore::Texture *texturePacmanDeath3;
    ICore::Texture *texturePacmanDeath4;
    ICore::Texture *texturePacmanDeath5;
    ICore::Texture *texturePacmanDeath6;
    ICore::Texture *texturePacmanDeath7;
    ICore::Texture *texturePacmanDeath8;
    ICore::Texture *texturePacmanDeath9;
    ICore::Texture *texturePacmanDeath10;
    ICore::Texture *texturePacmanDeath11;
    ICore::Texture *texturePacmanFaceDown1;
    ICore::Texture *texturePacmanFaceDown2;
    ICore::Texture *texturePacmanFaceLeft1;
    ICore::Texture *texturePacmanFaceLeft2;
    ICore::Texture *texturePacmanFaceRight1;
    ICore::Texture *texturePacmanFaceRight2;
    ICore::Texture *texturePacmanFaceUp1;
    ICore::Texture *texturePacmanFaceUp2;
    ICore::Texture *textureGhostEyesDown;
    ICore::Texture *textureGhostEyesLeft;
    ICore::Texture *textureGhostEyesRight;
    ICore::Texture *textureGhostEyesUp;
    ICore::Texture *textureGhostFrightenedBlue1;
    ICore::Texture *textureGhostFrightenedBlue2;
    ICore::Texture *textureGhostFrightenedWhite1;
    ICore::Texture *textureGhostFrightenedWhite2;
    ICore::Texture *textureBlinkyFaceRight2;
    ICore::Texture *textureBlinkyFaceDown1;
    ICore::Texture *textureBlinkyFaceDown2;
    ICore::Texture *textureBlinkyFaceLeft1;
    ICore::Texture *textureBlinkyFaceLeft2;
    ICore::Texture *textureBlinkyFaceUp1;
    ICore::Texture *textureBlinkyFaceUp2;
    ICore::Texture *texturePinkyFaceRight2;
    ICore::Texture *texturePinkyFaceDown1;
    ICore::Texture *texturePinkyFaceDown2;
    ICore::Texture *texturePinkyFaceLeft1;
    ICore::Texture *texturePinkyFaceLeft2;
    ICore::Texture *texturePinkyFaceUp1;
    ICore::Texture *texturePinkyFaceUp2;
    ICore::Texture *textureInkyFaceRight2;
    ICore::Texture *textureInkyFaceDown1;
    ICore::Texture *textureInkyFaceDown2;
    ICore::Texture *textureInkyFaceLeft1;
    ICore::Texture *textureInkyFaceLeft2;
    ICore::Texture *textureInkyFaceUp1;
    ICore::Texture *textureInkyFaceUp2;
    ICore::Texture *textureClydeFaceRight2;
    ICore::Texture *textureClydeFaceDown1;
    ICore::Texture *textureClydeFaceDown2;
    ICore::Texture *textureClydeFaceLeft1;
    ICore::Texture *textureClydeFaceLeft2;
    ICore::Texture *textureClydeFaceUp1;
    ICore::Texture *textureClydeFaceUp2;

    bool inputEnabled;
    bool isButtonPressed(ICore::Button button);

    static constexpr ICore::Vector2u displayCells = {28, 36};
    static constexpr ICore::Vector2u displayPixels = {PacmanGameModule::displayCells.x * 8, PacmanGameModule::displayCells.y * 8};

    // Get the distance from the given position to the nearest cell's middle
    static ICore::Vector2u distanceToCellMiddle(ICore::Vector2u pixelPosition);

    // Clamps a given cell to valid coordinates within the maze
    static ICore::Vector2u clampCellPosition(ICore::Vector2u cellPosition);

    // This entire thing is for the purposes of replicating the original hardware's cell frame buffer
    struct DrawableCellTexture {
        ICore::Texture *texture;
        ICore::Vector2u realPositionOffset;
    };
    struct DrawableCellCharacter {
        char character;
        ICore::Color color;
    };
    struct DrawableCellThing {
        std::variant<DrawableCellCharacter, DrawableCellTexture> thing;
        bool enabled;
    };
    std::unordered_map<ICore::Vector2u, DrawableCellThing, utils::HashVector2u> drawnCellThings;
    bool isValidCell(ICore::Vector2u cellPosition);
    void setCellCharacter(ICore::Vector2u cellPosition, char character, ICore::Color color = ICore::Color::white);
    void setCellSprite(ICore::Vector2u cellPosition, ICore::Texture *texture, ICore::Vector2u positionOffset = {0, 0});
    void setCellText(ICore::Vector2u cellPosition, std::string_view text, ICore::Color color = ICore::Color::white);
    void setCellScore(ICore::Vector2u cellPosition, std::uint64_t score);
    void setCellEnable(ICore::Vector2u cellPosition, bool enable);
    std::optional<DrawableCellThing> getCellDrawnThing(ICore::Vector2u cellPosition);

    // This reproduces the original hardware's sprite renderer
    struct Sprite {
        bool enabled = false;
        ICore::Texture *texture = nullptr;
        ICore::Vector2u position = {0, 0};
    };
    std::array<Sprite, 6> sprites;
    Sprite *const spritePacman = &PacmanGameModule::sprites[0];
    Sprite *const spriteBlinky = &PacmanGameModule::sprites[1];
    Sprite *const spritePinky = &PacmanGameModule::sprites[2];
    Sprite *const spriteInky = &PacmanGameModule::sprites[3];
    Sprite *const spriteClyde = &PacmanGameModule::sprites[4];
    Sprite *const spriteFruit = &PacmanGameModule::sprites[5];

    static ICore::Vector2u actorPositionToSpritePosition(ICore::Vector2u actorPosition);

    // Actual game state
    struct ActionTrigger {
        static constexpr std::uint64_t disabledFrame = std::numeric_limits<std::uint64_t>::max();

        void setNow(PacmanGameModule *game)
        {
            this->frame = game->currentFrame + 1;
        }
        void setAfter(PacmanGameModule *game, std::uint64_t frames)
        {
            this->frame = game->currentFrame + frames;
        }
        void disable()
        {
            this->frame = ActionTrigger::disabledFrame;
        }

        bool isNow(PacmanGameModule *game)
        {
            return this->frame == game->currentFrame;
        }
        std::uint64_t framesSince(PacmanGameModule *game)
        {
            if (game->currentFrame >= this->frame)
                return game->currentFrame - this->frame;
            return ActionTrigger::disabledFrame;
        }
        bool isExactlyAfter(PacmanGameModule *game, std::uint64_t frames)
        {
            return this->framesSince(game) == frames;
        }
        bool isAfter(PacmanGameModule *game, std::uint64_t frames)
        {
            auto since = this->framesSince(game);
            if (since == ActionTrigger::disabledFrame)
                return false;
            return since >= frames;
        }
        bool isBefore(PacmanGameModule *game, std::uint64_t frames)
        {
            auto since = this->framesSince(game);
            if (since == ActionTrigger::disabledFrame)
                return false;
            return since < frames;
        }

        std::uint64_t frame = 0;
    };

    struct IntroState {
        ActionTrigger triggerStart;
    } introState;

    struct GameState {
        ActionTrigger triggerStart;
        ActionTrigger triggerReady;
        ActionTrigger triggerRoundStarted;
        ActionTrigger triggerRoundWon;
        ActionTrigger triggerGameOver;

        // The last time Pac-Man ate a dot at
        ActionTrigger triggerAteDot;

        // The last time Pac-Man ate a pill at
        ActionTrigger triggerAtePill;

        // The last time Pac-Man ate a ghost at
        ActionTrigger triggerAteGhost;

        // The last time Pac-Man was killed (by a ghost) at
        ActionTrigger triggerPacmanKilled;

        // The last time Pac-Man ate a bonus fruit
        ActionTrigger triggerAteBonusFruit;

        // This will force the ghosts to exit the house after a while if Pac-Man just doesn't eat dots for too long
        ActionTrigger triggerHouseLeaveForce;

        // The last time a bonus fruit started being shown at
        ActionTrigger triggerActivateBonusFruit;

        // The curreent round id (starts at 0)
        std::uint32_t currentRound = 0;

        enum FreezeReason {
            freezeReasonPrelude = 1 << 0, // Currently doing game prelude
            freezeReasonReady = 1 << 1, // Currently doing the "READY!" thingy at the start of a round
            freezeReasonAteGhost = 1 << 2, // Pac-Man currently eating a ghost
            freezeReasonPacmanKilled = 1 << 3, // Pac-Man currently getting killed by a ghost
            freezeReasonWon = 1 << 4, // Currently finishing up the round after winning (i.e. having eaten all the dots)
        };
        int freezeReason = 0; // Is set to values from FreezeReason

        static constexpr std::int32_t initialLifeCount = 5;
        std::int32_t currentLives = 0;

        // This thing is here for the ghost house logic
        struct {
            bool active = false; // Is set when a life is lost
            std::uint8_t value = 0;
        } globalDotCounter;
        static constexpr std::uint32_t totalDotCount = 244;
        std::uint32_t dotsEatenCount = 0; // The round is won when dotsEatenCount == totalDotCount

        // Is the score, except divided by 10
        std::uint64_t score = 0;

        // The current high score as the game sees it (also divided by 10)
        std::uint64_t highScore = 0;

        enum class BonusFruit {
            none,
            cherries,
            strawberry,
            peach,
            apple,
            grapes,
            galaxian,
            bell,
            key,
            amountOfBonusFruits
        } activeBonusFruit;

        // Counts how many ghosts have been eaten since we've last eaten a pill
        std::uint8_t ghostsEatenCount = 0;

        // Set up specially so that we can know if the first bit is 0 then we're moving horizontally and otherwise vertically
        enum Direction {
            dirRight = 0b00,
            dirDown = 0b01,
            dirLeft = 0b10,
            dirUp = 0b11,
            dirCount = 0b100
        } dir = static_cast<Direction>(0);

        struct Actor {
            ICore::Vector2u position = {0, 0}; // This position is in pixel coordinates btw
            Direction currentDir;
            std::uint64_t animationFrame = 0; // This gets incremented every time the actor has moved in the current frame
        };

        static constexpr std::uint64_t freezeFramesAfterPacmanKilled = 60; // The amount of frames for which the game completely freezes after Pac-Man gets killed
        static constexpr std::uint64_t freezeFramesPacmanDeathSequence = 150; // The amount of frames for which the Pac-Man death thingy (where he shrivels up and stuff) lasts
        Actor pacman;

        static constexpr std::uint64_t freezeFramesAfterGhostEaten = 60;
        enum GhostType {
            ghostBlinky,
            ghostPinky,
            ghostInky,
            ghostClyde,
            ghostAmount
        };
        struct Ghost {
            Actor actor;
            GhostType type;

            enum class State {
                none,
                chase, // This means the ghost is trying to go after Pac-Man
                scatter, // This means the ghost is in "scatter" mode, i.e. will go to one of the scatter targets
                frightened, // Pac-Man has eaten a pill, i.e. the ghost will try to run away
                justEyes, // Pac-Man has eaten this ghost and it is going back to the ghost house
                inGhostHouse, // The ghost is currently in the ghost house
                leavingGhostHouse, // The ghost is currently leaving the ghost house
                enteringGhostHouse, // The ghost is currently entering the ghost house
            } state;

            // Ghosts decide on which cell they're going to move next one cell before actually doing so
            GameState::Direction nextDir;

            // Ghosts try to go to this cell
            ICore::Vector2u targetCell;

            // This determines when the ghost leaves the ghost house
            std::uint16_t dotCount;
            std::uint16_t dotLimit;

            ActionTrigger triggerFrightened; // Last time frightened mode was entered
            ActionTrigger triggerGotEaten; // Last time this ghost got eaten by Pac-Man
        } ghosts[static_cast<std::size_t>(PacmanGameModule::GameState::ghostAmount)];

        Ghost *blinky = &GameState::ghosts[ghostBlinky];
        Ghost *pinky = &GameState::ghosts[ghostPinky];
        Ghost *inky = &GameState::ghosts[ghostInky];
        Ghost *clyde = &GameState::ghosts[ghostClyde];

        // (in pixels)
        static constexpr ICore::Vector2u ghostStartingPositions[GameState::ghostAmount] = {
            [GameState::ghostBlinky] = {14 * 8, 14 * 8 + 4},
            [GameState::ghostPinky] = {14 * 8, 17 * 8 + 4},
            [GameState::ghostInky] = {12 * 8, 17 * 8 + 4},
            [GameState::ghostClyde] = {16 * 8, 17 * 8 + 4},
        };
        // (in pixels)
        static constexpr ICore::Vector2u ghostTargetPositionsInGhostHouse[GameState::ghostAmount] = {
            [GameState::ghostBlinky] = {14 * 8, 17 * 8 + 4},
            [GameState::ghostPinky] = {14 * 8, 17 * 8 + 4},
            [GameState::ghostInky] = {12 * 8, 17 * 8 + 4},
            [GameState::ghostClyde] = {16 * 8, 17 * 8 + 4},
        };
        // (in cells)
        static constexpr ICore::Vector2u ghostTargetPositionsScatter[GameState::ghostAmount] = {
            [GameState::ghostBlinky] = {25, 0},
            [GameState::ghostPinky] = {2, 0},
            [GameState::ghostInky] = {27, 34},
            [GameState::ghostClyde] = {0, 34},
        };

        static constexpr ICore::Vector2u ghostHousePixelEnterPoint = {14 * 8, 14 * 8 + 4};

        // TODO add speeds for Pac-Man and the ghosts and stuff like that
        struct LevelInformation {
            BonusFruit bonusFruit;
            std::uint16_t bonusFruitScore;
            std::uint16_t afraidFrames;
        };

        static constexpr LevelInformation levelInformationTable[] = {
            {BonusFruit::cherries, 10, 6 * 60},
            {BonusFruit::strawberry, 30, 5 * 60},
            {BonusFruit::peach, 50, 4 * 60},
            {BonusFruit::peach, 50, 3 * 60},
            {BonusFruit::apple, 70, 2 * 60},
            {BonusFruit::apple, 70, 5 * 60},
            {BonusFruit::grapes, 100, 2 * 60},
            {BonusFruit::grapes, 100, 2 * 60},
            {BonusFruit::galaxian, 200, 1 * 60},
            {BonusFruit::galaxian, 200, 6 * 60},
            {BonusFruit::bell, 300, 6 * 60},
            {BonusFruit::bell, 300, 6 * 60},
            {BonusFruit::key, 500, 1 * 60},
            {BonusFruit::key, 500, 3 * 60},
            {BonusFruit::key, 500, 1 * 60},
            {BonusFruit::key, 500, 1 * 60},
            {BonusFruit::key, 500, 1},
            {BonusFruit::key, 500, 1 * 60},
            {BonusFruit::key, 500, 1},
            {BonusFruit::key, 500, 1},
            {BonusFruit::key, 500, 1},
            // After the last level, this just repeats forever
        };

        static LevelInformation getLevelInformation(std::uint32_t round);

        static ICore::Vector2u directionToVector2u(GameState::Direction direction);
        static GameState::Direction directionReverse(GameState::Direction direction);
        static bool cellPositionIsInTunnel(ICore::Vector2u cellPosition);
        static bool cellPositionIsInRedzone(ICore::Vector2u cellPosition);
    } gameState;

    ICore::Texture *getBonusFruitTexture(PacmanGameModule::GameState::BonusFruit bonusFruit);

    // This is put here because we need the declaration of GameState::BonusFruit
    void setCellScoreFruit(GameState::BonusFruit bonusFruit);

    std::uint64_t currentFrame = 0; // Basic counter of how many frames have passed since the game was started

    enum class Mode {
        intro,
        game,
    } mode;

    void resetAllDraw();
    void updateIntro();
    void updateGame();
    void updateGameInit();
    void updateGameInitMaze();
    void updateGameInitRound();
    void updateGameDisableAllTimers();
    void updateGameActors();
    bool updateGameShouldPacmanMove();
    void updateGameDotEaten();
    void updateGameGhosthouseDotCounters();
    void updateGameGhostState(PacmanGameModule::GameState::Ghost *ghost);
    void updateGameGhostTarget(PacmanGameModule::GameState::Ghost *ghost);
    PacmanGameModule::GameState::Ghost::State updateGameGetGhostScatterOrChase();

    GameState::Direction updateGameGetInputDirection(GameState::Direction defaultDirection);

    // Determine if we can move from a given position (in pixels) to a wanted direction.
    // allowCornering is for the purposes of the special feature given to Pac-Man which allows him to take a sort of diagonal shortcut around corners
    bool updateGameCanMoveTo(ICore::Vector2u currentPosition, GameState::Direction wantedDirection, bool allowCornering);

    // This determines whether a given cell contains a dot
    bool updateGameIsCellDot(ICore::Vector2u cellPosition);

    // This determines whether a given cell contains a pill
    bool updateGameIsCellPill(ICore::Vector2u cellPosition);

    // This determines whether a given cell contains a cell that blocks actors
    bool updateGameIsCellBlocking(ICore::Vector2u cellPosition);

    // Determines the new pixel position for the actor, given a direction (note: this does NOT check for blocking cells)
    ICore::Vector2u updateGameMove(ICore::Vector2u position, GameState::Direction direction, bool allowCornering);

    unsigned updateGameGetGhostMoveSpeed(const PacmanGameModule::GameState::Ghost *ghost);

    // Computes the next direction for this ghost. This returns true if the movement to do now should ALWAYS occur without giving a single shit about blocking tiles or the ghost's position or whatever (for stuff like when we're in the ghost house)
    bool updateGameGhostDirection(PacmanGameModule::GameState::Ghost *ghost);

    // Animate cells
    void updateGameCells();

    // Animate sprites
    void updateGameSprites();

    // Normal Pac-Man animation
    void updateGameSpritePacman(GameState::Direction direction, std::uint64_t frame);

    // Pac-Man death animation
    void updateGameSpritePacmanDeath(std::uint64_t frame);

    // Ghost animation when it has been eaten
    void updateGameSpriteGhostJustEyes(GameState::GhostType ghostType, GameState::Direction direction);

    // Ghost animation while it is frightened
    void updateGameSpriteGhostFrightened(GameState::GhostType ghostType, std::uint64_t frame);

    // Ghost animation in a normal state
    void updateGameSpriteGhost(GameState::GhostType ghostType, GameState::Direction direction, std::uint64_t frame);

public:
    void init(ICore *coreHandle) override;
    void update() override;
    void draw() override;
};
