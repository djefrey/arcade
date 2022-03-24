#include "coreImpl.hpp"
#include "arcade-interface/IDisplayModule.hpp"
#include "menuGame.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>

CoreImpl::CoreImpl()
{
    this->initLibraryLists();

    std::ifstream scoreFile("scores.txt");
    std::string currentLine;
    while (std::getline(scoreFile, currentLine)) {
        CoreImpl::Score score;
        std::stringstream lineStream{currentLine};
        lineStream >> score.playerName >> score.value;
        this->scores.insert(score);
    }
}

CoreImpl::~CoreImpl()
{
    std::ofstream scoreFile("scores.txt");
    for (const auto &i : this->scores)
        scoreFile << i.playerName << i.value << '\n';
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
    };

    newTexture.displayModuleRawTexture = this->checkDisplayModuleNonNull()->loadTexture(pngFilename, character, characterColor, backgroundColor, width, height);
    if (newTexture.displayModuleRawTexture == nullptr)
        return nullptr;
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

void CoreImpl::changeDisplayModule(std::unique_ptr<IDisplayModule> displayModule)
{
    for (auto &i : this->textures)
        i.displayModuleRawTexture.reset();
    this->currentDisplayModule = std::move(displayModule);
    this->checkDisplayModuleNonNull()->setPixelsPerCell(this->lastSetPixelsPerCellArg);
    this->checkDisplayModuleNonNull()->openWindow(this->lastOpenWindowArg);
    for (auto &i : this->textures)
        i.displayModuleRawTexture = this->checkDisplayModuleNonNull()->loadTexture(i.pngFilename, i.character, i.characterColor, i.backgroundColor, i.width, i.height);
}

void CoreImpl::changeGameModule(std::unique_ptr<IGameModule> gameModule)
{
    this->textures.clear();
    this->currentGameModule = std::move(gameModule);
    this->currentGameModule->init(this);
}

void CoreImpl::initLibraryLists()
{
    for (auto &path : std::filesystem::directory_iterator("lib/")) {
        dl::Handle dynamicLib(path.path().filename().string());

        if (!dynamicLib.isLoaded())
            continue;

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

void CoreImpl::runMenu()
{
    auto menu = std::make_unique<MenuGame>();
    auto menuPtr = menu.get();

    this->changeGameModule(std::move(menu));
    while (this->currentGameModule.get() == menuPtr) {
        this->currentDisplayModule->update();
        this->currentGameModule->update();
        this->currentGameModule->draw();
        this->currentDisplayModule->display();
    }
}

void CoreImpl::runGame()
{
    throw std::runtime_error("TODO");
}
