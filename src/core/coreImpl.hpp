#pragma once

#include "arcade-interface/ICore.hpp"
#include "arcade-interface/IDisplayModule.hpp"
#include <deque>

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

    std::unique_ptr<IDisplayModule> displayModule;
    unsigned framerate;
    std::deque<CoreImpl::TextureImpl> textures;

    std::uint32_t lastSetPixelsPerCellArg;
    ICore::Vector2u lastOpenWindowArg;

public:
    void setPixelsPerCell(std::uint32_t pixelsPerCell) override;
    void setFramerate(unsigned framerate) override;
    ICore::Texture *loadTexture(const std::string &pngFilename, char character, ICore::Color characterColor, ICore::Color backgroundColor, std::size_t width, std::size_t height) override;
    
    void openWindow(ICore::Vector2u pixelsWantedWindowSize) override;
    bool isButtonPressed(ICore::Button button) override;
    MouseButtonReleaseEvent getMouseButtonReleaseEvent() override;

    void startTextInput() override;
    std::string getTextInput() override;
    void endTextInput() override;

    void renderSprite(ICore::Sprite sprite) override;

    void setDisplayModule(std::unique_ptr<IDisplayModule> displayModule);
};
