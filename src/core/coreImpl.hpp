#pragma once

#include "arcade-interface/ICore.hpp"
#include "arcade-interface/IDisplayModule.hpp"
#include "arcade-interface/IGameModule.hpp"
#include "dl.hpp"
#include <deque>
#include <optional>
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

    // gameLibraries and displayLibraries MUST be before currentDisplayModule and currentGameModule since otherwise the deleter for them will be gone (i.e. the handle to the dynamic library closed) by the time we try to delete those (since members are deleted in the opposite order of declaration)
    std::vector<std::pair<dl::Handle, std::unique_ptr<IGameModule>(*)()>> gameLibraries;
    std::vector<std::pair<dl::Handle, std::unique_ptr<IDisplayModule>(*)()>> displayLibraries;
    std::unique_ptr<IDisplayModule> currentDisplayModule;
    std::unique_ptr<IGameModule> currentGameModule;

    unsigned framerate;
    std::deque<CoreImpl::TextureImpl> textures;

    std::optional<std::uint32_t> lastSetPixelsPerCellArg;
    std::optional<ICore::Vector2u> lastOpenWindowArg;

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
    bool shouldExitNow();

    struct timespec timeFrameEnd;
    void setupSleep();
    void doSleep();

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
    bool menuNotifyIsFinished;
    std::size_t menuCurrentlySelectedGame = 0, menuCurrentlySelectedDisplay = 0;

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
