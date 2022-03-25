#pragma once

#include "arcade-interface/IGameModule.hpp"
#include "utils/textHandler.hpp"
#include "utils/vector2u.hpp"
#include <limits>
#include <vector>
#include <variant>

class PacmanGameModule : public virtual IGameModule {
    ICore *coreHandle = nullptr;
    utils::TextHandler textHandler;

    ICore::Texture *textureBlinkyFaceRight1;
    ICore::Texture *texturePinkyFaceRight1;
    ICore::Texture *textureInkyFaceRight1;
    ICore::Texture *textureClydeFaceRight1;
    ICore::Texture *textureDot;
    ICore::Texture *texturePill;
    ICore::Texture *texturePts;

    static constexpr ICore::Vector2u displayCells = {28, 36};
    static constexpr ICore::Vector2u displayPixels = {displayCells.x * 8, displayCells.y * 8};

    struct DrawableCellTexture {
        ICore::Texture *texture;
        ICore::Vector2u realPositionOffset;
    };
    struct DrawableCellCharacter {
        char character;
        ICore::Color color;
    };
    using DrawableCellThing = std::variant<DrawableCellCharacter, DrawableCellTexture>;
    std::unordered_map<ICore::Vector2u, DrawableCellThing, utils::HashVector2u> drawnCellThings;

    bool isValidCell(ICore::Vector2u cellPosition);
    void setCellCharacter(ICore::Vector2u cellPosition, char character, ICore::Color color = ICore::Color::white);
    void setCellSprite(ICore::Vector2u cellPosition, ICore::Texture *texture, ICore::Vector2u positionOffset = {0, 0});
    void setCellText(ICore::Vector2u cellPosition, std::string_view text, ICore::Color color = ICore::Color::white);
    void setCellScore(ICore::Vector2u cellPosition, std::uint64_t score);

    struct ActionTrigger {
        static constexpr std::uint64_t disabledFrame = std::numeric_limits<std::uint64_t>::max();

        void start(PacmanGameModule *game)
        {
            this->frame = game->currentFrame + 1;
        }
        bool isNow(PacmanGameModule *game)
        {
            return this->frame == game->currentFrame;
        }
        std::uint64_t framesSince(PacmanGameModule *game)
        {
            if (game->currentFrame >= this->frame)
                return game->currentFrame - this->frame;
            return this->disabledFrame;
        }
        bool isExactlyAfter(PacmanGameModule *game, std::uint64_t frames)
        {
            return this->framesSince(game) == frames;
        }
        bool isAfter(PacmanGameModule *game, std::uint64_t frames)
        {
            auto since = this->framesSince(game);
            if (since == this->disabledFrame)
                return false;
            return since >= frames;
        }

        std::uint64_t frame = 0;
    };

    ActionTrigger startIntroAction;
    ActionTrigger startGameAction;
    std::uint64_t currentFrame = 0;

    enum class Mode {
        intro,
        game,
    } mode;

    void resetAllDraw();
    void updateIntro();

public:
    void init(ICore *coreHandle) override;
    void update() override;
    void draw() override;
};
