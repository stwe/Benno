#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "BinaryFile.h"
#include "PaletteFile.h"

namespace sg::file
{
    struct BshTexture;

    class BshFile : public BinaryFile
    {
    public:
        //-------------------------------------------------
        // BSH filenames
        //-------------------------------------------------

        enum class BshFileNameId
        {
            EFFEKTE,
            FISCHE,
            GAUKLER,
            MAEHER,
            NUMBERS,
            SCHATTEN,
            SHIP,
            SOLDAT,
            STADTFLD,
            TIERE,
            TRAEGER,
            INVALID
        };

        inline static std::vector<BshFileNameId> const BSH_FILENAME_ID_TABLE{
            BshFileNameId::EFFEKTE, BshFileNameId::FISCHE, BshFileNameId::GAUKLER,
            BshFileNameId::MAEHER, BshFileNameId::NUMBERS, BshFileNameId::SCHATTEN,
            BshFileNameId::SHIP, BshFileNameId::SOLDAT, BshFileNameId::STADTFLD,
            BshFileNameId::TIERE, BshFileNameId::TRAEGER
        };

        inline static std::unordered_map<BshFileNameId, std::string> const BSH_FILENAME_ID_STRINGS{
            { BshFileNameId::EFFEKTE, "EFFEKTE" },
            { BshFileNameId::FISCHE, "FISCHE" },
            { BshFileNameId::GAUKLER, "GAUKLER" },
            { BshFileNameId::MAEHER, "MAEHER" },
            { BshFileNameId::NUMBERS, "NUMBERS" },
            { BshFileNameId::SCHATTEN, "SCHATTEN" },
            { BshFileNameId::SHIP, "SHIP" },
            { BshFileNameId::SOLDAT, "SOLDAT" },
            { BshFileNameId::STADTFLD, "STADTFLD" },
            { BshFileNameId::TIERE, "TIERE" },
            { BshFileNameId::TRAEGER, "TRAEGER" },
        };

        static const std::string& BshFileNameIdToString(const BshFileNameId t_id)
        {
            return BSH_FILENAME_ID_STRINGS.find(t_id)->second;
        }

        inline static std::unordered_map<std::string, BshFileNameId> const STRING_BSH_FILENAME_IDS{
            { "EFFEKTE", BshFileNameId::EFFEKTE },
            { "FISCHE" , BshFileNameId::FISCHE },
            { "GAUKLER", BshFileNameId::GAUKLER },
            { "MAEHER", BshFileNameId::MAEHER },
            { "NUMBERS", BshFileNameId::NUMBERS },
            { "SCHATTEN", BshFileNameId::SCHATTEN },
            { "SHIP", BshFileNameId::SHIP },
            { "SOLDAT", BshFileNameId::SOLDAT },
            { "STADTFLD" , BshFileNameId::STADTFLD},
            { "TIERE", BshFileNameId::TIERE },
            { "TRAEGER", BshFileNameId::TRAEGER },
        };

        static BshFileNameId StringToBshFileNameId(const std::string& t_idString)
        {
            const auto it{ STRING_BSH_FILENAME_IDS.find(t_idString) };
            return it == STRING_BSH_FILENAME_IDS.end() ?
                BshFileNameId::INVALID : it->second;
        }

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
