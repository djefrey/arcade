#pragma once

#include "arcade-interface/ICore.hpp"
#include "arcade-interface/IDisplayModule.hpp"
#include "arcade-interface/IGameModule.hpp"
#include "dl.hpp"
#include <deque>
#include <set>
#include <vector>
#include <tuple>

class CoreImpl : public virtual ICore {
    IDisplayModule *checkDisplayModuleNonNull();

    struct TextureImpl {
        std::string pngFilename;
        char character;
        ICore::Color characterColor;
        ICore::Color backgroundColor;
        std::size_t width;
        std::size_t height;

        std::unique_ptr<IDisplayModule::RawTexture> displayModuleRawTexture;
    };

    std::unique_ptr<IDisplayModule> currentDisplayModule;
    std::unique_ptr<IGameModule> currentGameModule;
    unsigned framerate;
    std::deque<CoreImpl::TextureImpl> textures;

    std::uint32_t lastSetPixelsPerCellArg;
    ICore::Vector2u lastOpenWindowArg;

    std::vector<std::pair<dl::Handle, std::unique_ptr<IGameModule>(*)()>> gameLibraries;
    std::vector<std::pair<dl::Handle, std::unique_ptr<IDisplayModule>(*)()>> displayLibraries;

    struct Score {
        std::uint32_t value;
        std::string playerName;

        bool operator<(const Score &other) const
        {
            return std::tie(value, playerName) < std::tie(other.value, other.playerName);
        }
    };
    std::multiset<Score> scores;

    void initLibraryLists();

public:
    CoreImpl();
    ~CoreImpl();

    void setPixelsPerCell(std::uint32_t pixelsPerCell) override;
    void setFramerate(unsigned framerate) override;
    ICore::Texture *loadTexture(const std::string &pngFilename, char character, ICore::Color characterColor, ICore::Color backgroundColor, std::size_t width, std::size_t height) override;
    
    void openWindow(ICore::Vector2u pixelsWantedWindowSize) override;
    bool isButtonPressed(ICore::Button button) override;
    MouseButtonReleaseEvent getMouseButtonReleaseEvent() override;

    void startTextInput() override;
    std::string getTextInput() override;
    void endTextInput() override;

    void clearScreen(ICore::Color color) override;
    void renderSprite(ICore::Sprite sprite) override;

    void addNewScore(std::uint32_t score) override;

    void changeDisplayModule(std::unique_ptr<IDisplayModule> displayModule);
    void changeGameModule(std::unique_ptr<IGameModule> gameModule);

    void runMenu();
    void runGame();

    const decltype(gameLibraries) &getGameLibraries() const
    {
        return this->gameLibraries;
    }

    const decltype(displayLibraries) &getDisplayLibraries() const
    {
        return this->displayLibraries;
    }

    const decltype(scores) &getScores() const
    {
        return this->scores;
    }

    std::string playerName = "Unknown";
};
