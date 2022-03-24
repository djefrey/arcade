#pragma once

#include "arcade-interface/ICore.hpp"
#include <stdexcept>

namespace utils {
    class TextHandler {
        ICore *coreHandle;
        std::uint32_t letterSize;
        std::string fontDirectory;
        std::unordered_map<char, ICore::Texture *> letters;
        bool isUppercaseOnly;

        std::string makeFilename(char character)
        {
            if (character != '/')
                return std::string(1, character);
            return "slash";
        }
        
    public:
        void init(ICore *coreHandle, std::size_t letterSize, std::string fontDirectory, bool isUppercaseOnly)
        {
            this->coreHandle = coreHandle;
            this->letterSize = letterSize;
            this->fontDirectory = fontDirectory;
            this->isUppercaseOnly = isUppercaseOnly;
        }

        void drawText(std::string_view text, size_t maxLength, ICore::Vector2u position)
        {
            for (std::size_t i = 0; i < text.length() && i < maxLength; ++i) {
                char textureCharacter = text[i];

                if (textureCharacter == ' ' || textureCharacter == '-' || textureCharacter == '&'  || textureCharacter == '_')
                    continue;

                if (this->isUppercaseOnly)
                    textureCharacter = std::toupper(textureCharacter);

                ICore::Texture **texturePtr = &this->letters[textureCharacter];
                if (*texturePtr == nullptr)
                    *texturePtr = this->coreHandle->loadTexture(this->fontDirectory + '/' + makeFilename(textureCharacter) + ".png", text[i], ICore::Color::black, ICore::Color::white, this->letterSize, this->letterSize);
                if (*texturePtr == nullptr)
                    *texturePtr = this->coreHandle->loadTexture(this->fontDirectory + "/unknown.png", '_', ICore::Color::black, ICore::Color::white, this->letterSize, this->letterSize);
                if (*texturePtr == nullptr)
                    throw std::runtime_error(std::string("Couldn't find font image for '") + text[i] + "' and '" + this->fontDirectory + ".png' could not be found");
                this->coreHandle->renderSprite({{position.x + (static_cast<std::uint32_t>(i) * this->letterSize), position.y}, *texturePtr});
            }
        }
    };
};
