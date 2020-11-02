#pragma once

#include <vector>

namespace sg::file
{
    struct BshTexture
    {
        std::vector<PaletteFile::Color32Bit> texturePixels;
        int width { 0 };
        int height{ 0 };
        uint32_t textureId{ 0 };
    };
}