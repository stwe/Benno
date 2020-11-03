#pragma once

#include <string>
#include <memory>
#include "BinaryFile.h"
#include "PaletteFile.h"

namespace sg::file
{
    struct BshTexture;

    class BshFile : public BinaryFile
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        BshFile() = delete;

        BshFile(const std::string& t_filePath, const std::vector<PaletteFile::Color32Bit>& t_palette);

        BshFile(const BshFile& t_other) = delete;
        BshFile(BshFile&& t_other) noexcept = delete;
        BshFile& operator=(const BshFile& t_other) = delete;
        BshFile& operator=(BshFile&& t_other) noexcept = delete;

        ~BshFile();

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] const BshTexture& GetBshTexture(int t_index) const;

        //-------------------------------------------------
        // BinaryFile Interface
        //-------------------------------------------------

        void ReadContentFromChunkData() override;

    protected:

    private:
        static constexpr auto NUMBER_OF_CHUNKS{ 1 };
        inline static const std::string CHUNK_ID{ "BSH" };

        static constexpr uint8_t END_MARKER{ 255 };
        static constexpr uint8_t END_OF_ROW{ 254 };

        std::vector<PaletteFile::Color32Bit> m_palette;

        std::vector<uint32_t> m_offsets;
        std::vector<std::unique_ptr<BshTexture>> m_bshTextures;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void DecodeTextures();
        void DecodeTexture(uint32_t t_offset);
        void CreateGLTextures();

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;
    };
}
