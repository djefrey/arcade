#pragma once

#include "arcade-interface/ICore.hpp"
#include <functional>
#include <cstdint>
#include <cstddef>

namespace utils {
    inline ICore::Vector2u addVec2u(ICore::Vector2u a, ICore::Vector2u b)
    {
        return {
            a.x + b.x,
            a.y + b.y,
        };
    }
    inline ICore::Vector2u subVec2u(ICore::Vector2u a, ICore::Vector2u b)
    {
        return {
            a.x - b.x,
            a.y - b.y,
        };
    }
    
    inline ICore::Vector2u mulVec2u(ICore::Vector2u a, std::int32_t b)
    {
        return {
            a.x * b,
            a.y * b,
        };
    }
    
    inline bool eqVec2u(ICore::Vector2u a, ICore::Vector2u b)
    {
        return (a.x == b.x) && (a.y == b.y);
    }

    // Same as eqVec2u, except this allows for some margin in cases where that's relevant
    inline bool almostEqVec2u(ICore::Vector2u a, ICore::Vector2u b, std::uint32_t margin)
    {
        return
            (std::abs(static_cast<std::int32_t>(a.x - b.x)) <= margin) &&
            (std::abs(static_cast<std::int32_t>(a.y - b.y)) <= margin);
    }

    inline std::int32_t squareDistanceVec2u(ICore::Vector2u a, ICore::Vector2u b)
    {
        const auto tmp = utils::subVec2u(b, a);
        return tmp.x * tmp.x + tmp.y * tmp.y;
    }
    
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
