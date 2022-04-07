#include "coreImpl.hpp"
#include "arcade-interface/IDisplayModule.hpp"
#include "menuGame.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <cassert>

CoreImpl::CoreImpl()
{
    this->initLibraryLists();

    std::ifstream scoreFile("scores.txt");
    std::string currentLine;
    while (std::getline(scoreFile, currentLine)) {
        CoreImpl::Score score;
        std::size_t i = currentLine.size() - 1;
        for (; i != std::size_t(-1); --i)
            if (!isdigit(currentLine[i]) && currentLine[i] != '-')
                break;
        score.value = std::strtol(currentLine.substr(i + 1).c_str(), nullptr, 0);
        score.playerName = currentLine.substr(0, i);
        this->scores.insert(score);
    }
}

CoreImpl::~CoreImpl()
{
    std::ofstream scoreFile("scores.txt");
    for (const auto &i : this->scores)
        scoreFile << i.playerName << ' ' << i.value << '\n';
}

IDisplayModule *CoreImpl::checkDisplayModuleNonNull()
{
    auto result = this->currentDisplayModule.get();
    if (result == nullptr)
        throw std::runtime_error("Called ICore method requiring an IDisplayModule without any IDisplayModule attached !");
    return result;
}

void CoreImpl::setPixelsPerCell(std::uint32_t pixelsPerCell)
{
    this->checkDisplayModuleNonNull()->setPixelsPerCell(pixelsPerCell);
    this->lastSetPixelsPerCellArg = pixelsPerCell;
}

void CoreImpl::setFramerate(unsigned int framerate)
{
    this->framerate = framerate;
}

ICore::Texture *CoreImpl::loadTexture(const std::string &pngFilename, char character, ICore::Color characterColor, ICore::Color backgroundColor, std::size_t width, std::size_t height)
{
    CoreImpl::TextureImpl newTexture = {
        .pngFilename = pngFilename,
        .character = character,
        .characterColor = characterColor,
        .backgroundColor = backgroundColor,
        .width = width,
        .height = height,
        .displayModuleRawTexture = nullptr
    };

    newTexture.displayModuleRawTexture = this->checkDisplayModuleNonNull()->loadTexture(pngFilename, character, characterColor, backgroundColor, width, height);
    assert(newTexture.displayModuleRawTexture != nullptr);
    this->textures.push_back(std::move(newTexture));
    return (ICore::Texture *)&this->textures.back();
}

void CoreImpl::openWindow(ICore::Vector2u pixelsWantedWindowSize)
{
    this->checkDisplayModuleNonNull()->openWindow(pixelsWantedWindowSize);
    this->lastOpenWindowArg = pixelsWantedWindowSize;
}

bool CoreImpl::isButtonPressed(ICore::Button button)
{
    return this->checkDisplayModuleNonNull()->isButtonPressed(button);
}

ICore::MouseButtonReleaseEvent CoreImpl::getMouseButtonReleaseEvent()
{
    return this->checkDisplayModuleNonNull()->getMouseButtonReleaseEvent();
}

void CoreImpl::startTextInput()
{
    return this->checkDisplayModuleNonNull()->startTextInput();
}

std::string CoreImpl::getTextInput()
{
    return this->checkDisplayModuleNonNull()->getTextInput();
}

void CoreImpl::endTextInput()
{
    this->checkDisplayModuleNonNull()->endTextInput();
}

void CoreImpl::clearScreen(ICore::Color color)
{
    this->checkDisplayModuleNonNull()->clearScreen(color);
}

void CoreImpl::renderSprite(ICore::Sprite sprite)
{
    IDisplayModule::Sprite displayModuleSprite = {
        .rawPixelPosition = sprite.pixelPosition,
        .texture = reinterpret_cast<CoreImpl::TextureImpl *>(sprite.texture)->displayModuleRawTexture.get()
    };
    this->checkDisplayModuleNonNull()->renderSprite(displayModuleSprite);
}

void CoreImpl::addNewScore(std::uint32_t score)
{
    this->scores.insert({score, this->playerName});
}

const std::string &CoreImpl::getPlayerName()
{
    return this->playerName;
}

void CoreImpl::changeDisplayModule(std::unique_ptr<IDisplayModule> displayModule)
{
    for (auto &i : this->textures)
        i.displayModuleRawTexture.reset();
    this->currentDisplayModule = std::move(displayModule);
    if (this->lastSetPixelsPerCellArg.has_value())
        this->checkDisplayModuleNonNull()->setPixelsPerCell(*this->lastSetPixelsPerCellArg);
    if (this->lastOpenWindowArg.has_value())
        this->checkDisplayModuleNonNull()->openWindow(*this->lastOpenWindowArg);
    for (auto &i : this->textures) {
        i.displayModuleRawTexture = this->checkDisplayModuleNonNull()->loadTexture(i.pngFilename, i.character, i.characterColor, i.backgroundColor, i.width, i.height);
        assert(i.displayModuleRawTexture != nullptr);
    }
}

void CoreImpl::changeGameModule(std::unique_ptr<IGameModule> gameModule)
{
    this->textures.clear();
    this->lastOpenWindowArg.reset();
    this->lastSetPixelsPerCellArg.reset();
    this->changeDisplayModuleToCurrentlySelected();
    this->currentGameModule = std::move(gameModule);
    this->currentGameModule->init(this);
}

void CoreImpl::changeDisplayModuleToCurrentlySelected()
{
    if (!this->isInMenu)
        this->changeDisplayModule(this->getDisplayLibraries().at(this->currentlySelectedDisplay).second());
}

void CoreImpl::changeGameModuleToCurrentlySelected()
{
    this->changeGameModule(this->getGameLibraries().at(this->currentlySelectedGame).second());
}

void CoreImpl::initLibraryLists()
{
    for (auto &path : std::filesystem::directory_iterator("lib/")) {
        dl::Handle dynamicLib(path.path().string());

        if (!dynamicLib.isLoaded()) {
            std::cerr << "Invalid .so (or non-.so file) found in lib/: '" << path.path().string() << "' (error: '" << dynamicLib.getLastError() << "')\n";
            continue;
        }

        auto gEpitechArcadeGetDisplayModuleHandlePtr =
            reinterpret_cast<decltype(&gEpitechArcadeGetDisplayModuleHandle)>(
                dynamicLib.lookupSymbol("gEpitechArcadeGetDisplayModuleHandle"));
        if (gEpitechArcadeGetDisplayModuleHandlePtr != nullptr) {
            this->displayLibraries.emplace_back(std::move(dynamicLib), gEpitechArcadeGetDisplayModuleHandlePtr);
            continue;
        }

        auto gEpitechArcadeGetGameModuleHandlePtr =
            reinterpret_cast<decltype(&gEpitechArcadeGetGameModuleHandle)>(
                dynamicLib.lookupSymbol("gEpitechArcadeGetGameModuleHandle"));
        if (gEpitechArcadeGetGameModuleHandlePtr != nullptr) {
            this->gameLibraries.emplace_back(std::move(dynamicLib), gEpitechArcadeGetGameModuleHandlePtr);
            continue;
        }
    }
    if (this->gameLibraries.size() == 0)
        throw std::runtime_error("Could not find any game to play...");
    if (this->displayLibraries.size() == 0)
        throw std::runtime_error("Could not find any display libraries to use...");
}

bool CoreImpl::shouldExitNow()
{
    return this->isButtonPressed(ICore::Button::F7) || this->checkDisplayModuleNonNull()->isClosing();
}

void CoreImpl::runMenu()
{
    this->isInMenu = true;
    auto menu = std::make_unique<MenuGame>();

    this->menuNotifyIsFinished = false;
    this->changeGameModule(std::move(menu));
    while (!this->menuNotifyIsFinished && !this->shouldExitNow()) {
        this->setupSleep();
        this->currentDisplayModule->update();
        this->currentGameModule->update();
        this->currentGameModule->draw();
        this->currentDisplayModule->display();
        this->doSleep();
    }
    this->isInMenu = false;
    if (this->menuNotifyIsFinished)
        this->changeGameModuleToCurrentlySelected(); // (note: this is why we can't just do this in the game, as that would result in a use-after-free)
}

void CoreImpl::decrementCurrentlySelectedGame()
{
    if (this->currentlySelectedGame == 0)
        this->currentlySelectedGame = this->getGameLibraries().size() - 1;
    else
        --this->currentlySelectedGame;
}

void CoreImpl::incrementCurrentlySelectedGame()
{
    ++this->currentlySelectedGame;
    this->currentlySelectedGame %= this->getGameLibraries().size();
}

void CoreImpl::decrementCurrentlySelectedDisplay()
{
    if (this->currentlySelectedDisplay == 0)
        this->currentlySelectedDisplay = this->getDisplayLibraries().size() - 1;
    else
        --this->currentlySelectedDisplay;
}

void CoreImpl::incrementCurrentlySelectedDisplay()
{
    ++this->currentlySelectedDisplay;
    this->currentlySelectedDisplay %= this->getDisplayLibraries().size();
}

void CoreImpl::checkAlwaysMappedKeysInGame()
{
    if (this->checkDisplayModuleNonNull()->isButtonPressed(IDisplayModule::Button::F1)) {
        this->decrementCurrentlySelectedDisplay();
        this->changeDisplayModuleToCurrentlySelected();
    }
    if (this->checkDisplayModuleNonNull()->isButtonPressed(IDisplayModule::Button::F2)) {
        this->incrementCurrentlySelectedDisplay();
        this->changeDisplayModuleToCurrentlySelected();
    }
    if (this->checkDisplayModuleNonNull()->isButtonPressed(IDisplayModule::Button::F3)) {
        this->decrementCurrentlySelectedGame();
        this->changeGameModuleToCurrentlySelected();
    }
    if (this->checkDisplayModuleNonNull()->isButtonPressed(IDisplayModule::Button::F4)) {
        this->incrementCurrentlySelectedGame();
        this->changeGameModuleToCurrentlySelected();
    }
    if (this->checkDisplayModuleNonNull()->isButtonPressed(IDisplayModule::Button::F5))
        this->changeGameModuleToCurrentlySelected();
    if (this->checkDisplayModuleNonNull()->isButtonPressed(IDisplayModule::Button::F6))
        this->runMenu();
}

void CoreImpl::runGame()
{
    while (!this->shouldExitNow()) {
        this->setupSleep();
        this->currentDisplayModule->update();
        this->checkAlwaysMappedKeysInGame();
        this->currentGameModule->update();
        this->currentGameModule->draw();
        this->currentDisplayModule->display();
        this->doSleep();
    }
}

void CoreImpl::setupSleep()
{
    clock_gettime(CLOCK_REALTIME, &this->timeFrameEnd);
    this->timeFrameEnd.tv_nsec += 16666666;
    if (this->timeFrameEnd.tv_nsec > 999999999) {
        this->timeFrameEnd.tv_nsec -= 1000000000;
        ++this->timeFrameEnd.tv_sec;
    }
}

void CoreImpl::doSleep()
{
    while (clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &this->timeFrameEnd, nullptr) != 0)
        continue;
}
