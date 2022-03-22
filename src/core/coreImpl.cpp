#include "coreImpl.hpp"
#include "arcade-interface/IDisplayModule.hpp"
#include <stdexcept>

IDisplayModule *CoreImpl::checkDisplayModuleNonNull()
{
    auto result = this->displayModule.get();
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

void CoreImpl::renderSprite(ICore::Sprite sprite)
{
    IDisplayModule::Sprite displayModuleSprite = {
        .rawPixelPosition = sprite.pixelPosition,
        .texture = reinterpret_cast<CoreImpl::TextureImpl *>(sprite.texture)->displayModuleRawTexture.get()
    };
    this->checkDisplayModuleNonNull()->renderSprite(displayModuleSprite);
}

void CoreImpl::setDisplayModule(std::unique_ptr<IDisplayModule> displayModule)
{
    for (auto &i : this->textures)
        i.displayModuleRawTexture.reset();
    this->displayModule = std::move(displayModule);
    this->checkDisplayModuleNonNull()->setPixelsPerCell(this->lastSetPixelsPerCellArg);
    this->checkDisplayModuleNonNull()->openWindow(this->lastOpenWindowArg);
    for (auto &i : this->textures)
        i.displayModuleRawTexture = this->checkDisplayModuleNonNull()->loadTexture(i.pngFilename, i.character, i.characterColor, i.backgroundColor, i.width, i.height);
}
