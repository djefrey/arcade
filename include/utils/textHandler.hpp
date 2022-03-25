#pragma once

#include "arcade-interface/ICore.hpp"
#include <stdexcept>

namespace utils {
    class TextHandler {
        ICore *coreHandle;
        std::uint32_t letterSize;
        std::string fontFilename;

        struct TextCharacter {
            char character;
            ICore::Color color;

            bool operator==(const TextCharacter &other) const
            {
                return std::tie(this->character, this->color) == std::tie(other.character, other.color);
            }

            struct HashFunction {
                std::size_t operator()(const TextCharacter &textChar) const
                {
                    return std::hash<char>()(textChar.character) ^ std::hash<int>()(static_cast<int>(textChar.color));
                }
            };
        };
        std::unordered_map<TextCharacter, ICore::Texture *, TextCharacter::HashFunction> letters;

    public:
        void init(ICore *coreHandle, std::size_t letterSize, std::string fontFilename)
        {
            this->coreHandle = coreHandle;
            this->letterSize = letterSize;
            this->fontFilename = fontFilename;
        }

        void drawText(std::string_view text, size_t maxLength, ICore::Vector2u position, ICore::Color color = ICore::Color::white)
        {
            for (std::size_t i = 0; i < text.length() && i < maxLength; ++i) {
                char textureCharacter = text[i];

                ICore::Texture **texturePtr = &this->letters[{textureCharacter, color}];
                if (*texturePtr == nullptr)
                    *texturePtr = this->coreHandle->loadTexture(this->fontFilename, text[i], color, ICore::Color::black, this->letterSize, this->letterSize);
                if (*texturePtr == nullptr)
                    throw std::runtime_error(std::string("Could not create '") + textureCharacter + "' texture");
                this->coreHandle->renderSprite({{position.x + (static_cast<std::uint32_t>(i) * this->letterSize), position.y}, *texturePtr});
            }
        }
    };
};
