#pragma once

#include <string>
#include <vector>
#include "BinaryFile.h"

namespace sg::file
{
    class PaletteFile : public BinaryFile
    {
    public:
        using Color32Bit = int32_t;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        PaletteFile() = delete;

        explicit PaletteFile(const std::string& t_filePath);

        PaletteFile(const PaletteFile& t_other) = delete;
        PaletteFile(PaletteFile&& t_other) noexcept = delete;
        PaletteFile& operator=(const PaletteFile& t_other) = delete;
        PaletteFile& operator=(PaletteFile&& t_other) noexcept = delete;

        ~PaletteFile() = default;

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] const std::vector<Color32Bit>& GetPalette() const noexcept;

        //-------------------------------------------------
        // BinaryFile Interface
        //-------------------------------------------------

        void ReadContentFromChunkData() override;

    protected:

    private:
        static constexpr auto NUMBER_OF_COLORS{ 256 };
        static constexpr auto NUMBER_OF_CHUNKS{ 1 };
        inline static const std::string CHUNK_ID{ "COL" };

        std::vector<Color32Bit> m_palette;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static Color32Bit RgbToInt(uint8_t t_red, uint8_t t_green, uint8_t t_blue);
    };
}
