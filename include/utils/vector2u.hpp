#pragma once

#include "arcade-interface/ICore.hpp"
#include <functional>
#include <cstdint>
#include <cstddef>

namespace utils {
    struct HashVector2u {
        std::size_t operator()(const ICore::Vector2u &vec) const
        {
            return std::hash<std::uint32_t>()(vec.x) ^ std::hash<std::uint32_t>()(vec.y);
        }
    };
};

inline bool operator==(const ICore::Vector2u &a, const ICore::Vector2u &b)
{
    return a.x == b.x &&
        a.y == b.y;
}
