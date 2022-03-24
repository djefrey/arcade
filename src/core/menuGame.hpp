#pragma once

#include "arcade-interface/IGameModule.hpp"
#include "utils/textHandler.hpp"
#include "coreImpl.hpp"

class MenuGame : virtual public IGameModule {
    CoreImpl *coreHandle;

    ICore::Texture *boxTopLeft;
    ICore::Texture *boxTopRight;
    ICore::Texture *boxBottomLeft;
    ICore::Texture *boxBottomRight;
    ICore::Texture *boxLeft;
    ICore::Texture *boxRight;
    ICore::Texture *boxTop;
    ICore::Texture *boxBottom;
    utils::TextHandler textHandler;

    std::size_t currentlySelectedGame = 0;
    std::size_t currentlySelectedDisplay = 0;
    bool isGettingText = false;

    void handleNormalInput();
    void handleTextInput();
    void startGettingText();
    void endGettingText();
public:
    void init(ICore *coreHandle);
    void update();
    void draw();
    void drawBox(ICore::Vector2u cellPosition, ICore::Vector2u boxSize);
};
