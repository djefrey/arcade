#pragma once

#include "arcade-interface/ICore.hpp"

namespace utils {
    // This function converts a raw position (i.e. a position as measured in pixels) into a cell position (i.e. a position as measured in cells)
    inline ICore::Vector2u rawPixelPositionToCellPosition(ICore::Vector2u rawPosition, std::uint32_t pixelsPerCell)
    {
        return {rawPosition.x / pixelsPerCell, rawPosition.y / pixelsPerCell};
    }

    // This function converts a cell position to a raw position
    inline ICore::Vector2u cellPositionToRawPixelPosition(ICore::Vector2u cellPosition, std::uint32_t pixelsPerCell)
    {
        return {cellPosition.x * pixelsPerCell, cellPosition.y * pixelsPerCell};
    }

    ICore::Vector2u getSquareCenter(ICore::Vector2u squareTopLeftPos, ICore::Vector2u squareSize)
    {
        return {squareTopLeftPos.x + ((squareTopLeftPos.x - squareSize.x) / 2), squareTopLeftPos.y + ((squareTopLeftPos.y - squareSize.y) / 2)};
    }
};