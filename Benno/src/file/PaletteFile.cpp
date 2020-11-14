#include "PaletteFile.h"
#include "Log.h"
#include "SgAssert.h"
#include "SgException.h"
#include "chunk/Chunk.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::file::PaletteFile::PaletteFile(const std::string& t_filePath)
    : BinaryFile(t_filePath)
{
    Log::SG_LOG_DEBUG("[PaletteFile::PaletteFile()] Creates PaletteFile object from file {}.", t_filePath);

    if (GetNumberOfChunks() != NUMBER_OF_CHUNKS)
    {
        throw SG_EXCEPTION("[PaletteFile::PaletteFile()] Invalid number of Chunks.");
    }

    if (!ChunkIndexHasId(0, CHUNK_ID))
    {
        throw SG_EXCEPTION("[PaletteFile::PaletteFile()] Invalid Chunk Id.");
    }
}

//-------------------------------------------------
// Getter / read-only
//-------------------------------------------------

const std::vector<sg::file::PaletteFile::Color32Bit>& sg::file::PaletteFile::GetPalette() const noexcept
{
    return m_palette;
}

//-------------------------------------------------
// BinaryFile Interface
//-------------------------------------------------

void sg::file::PaletteFile::ReadContentFromChunkData()
{
    Log::SG_LOG_DEBUG("[PaletteFile::ReadContentFromChunkData()] Start reading Palette data from Chunks...");

    const auto& chunk0{ GetChunk(0) };

    for (size_t i{ 0 }; i < chunk0.GetLength(); i += 4)
    {
        const auto red{ static_cast<uint8_t>(chunk0.GetData().at(i)) };
        const auto green{ static_cast<uint8_t>(chunk0.GetData().at(i + 1)) };
        const auto blue{ static_cast<uint8_t>(chunk0.GetData().at(i + 2)) };

        m_palette.push_back(RgbToInt(red, green, blue));
    }

    SG_ASSERT(m_palette.size() == NUMBER_OF_COLORS, "[PaletteFile::ReadContentFromChunkData()] Invalid number of colors.");

    Log::SG_LOG_DEBUG("[PaletteFile::ReadContentFromChunkData()] Palette data read successfully.");
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

sg::file::PaletteFile::Color32Bit sg::file::PaletteFile::RgbToInt(const uint8_t t_red, const uint8_t t_green, const uint8_t t_blue)
{
    const Color32Bit alpha{ 255 };

    return (alpha << 24) |
        (t_red << 16) |
        (t_green << 8) |
        (t_blue);
}
