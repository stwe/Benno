#include "BshFile.h"
#include "BshTexture.h"
#include "Log.h"
#include "SgException.h"
#include "OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::file::BshFile::BshFile(const std::string& t_filePath, const std::vector<PaletteFile::Color32Bit>& t_palette)
    : BinaryFile(t_filePath)
    , m_palette{ t_palette }
{
    Log::SG_LOG_DEBUG("[BshFile::BshFile()] Creates BshFile object from file {}.", t_filePath);

    if (GetNumberOfChunks() != NUMBER_OF_CHUNKS)
    {
        throw SG_EXCEPTION("[BshFile::BshFile()] Invalid number of Chunks.");
    }

    if (ChunkIndexHasId(0, CHUNK_ID))
    {
        throw SG_EXCEPTION("[BshFile::BshFile()] Invalid Chunk Id.");
    }
}

//-------------------------------------------------
// Getter / read-only
//-------------------------------------------------

const sg::file::BshTexture& sg::file::BshFile::GetBshTexture(const int t_index) const
{
    return *m_bshTextures[t_index];
}

//-------------------------------------------------
// BinaryFile Interface
//-------------------------------------------------

void sg::file::BshFile::ReadContentFromChunkData()
{
    Log::SG_LOG_DEBUG("[BshFile::ReadContentFromChunkData()] Start reading BSH data from Chunks...");

    // get pointer to the first element
    const auto* dataPtr{ reinterpret_cast<const uint32_t*>(GetChunk(0).GetData().data()) };

    // read and store the first offset
    const auto firstOffset{ *dataPtr };
    m_offsets.push_back(firstOffset);

    // calc number of textures
    const auto count{ firstOffset / 4 };

    // store other offsets
    for (auto i{ 1u }; i < count; ++i)
    {
        m_offsets.push_back(dataPtr[i]);
    }

    Log::SG_LOG_DEBUG("[BshFile::ReadContentFromChunkData()] Detected {} texture offsets.", m_offsets.size());

    DecodeTextures();
    CreateGLTextures();

    Log::SG_LOG_DEBUG("[BshFile::ReadContentFromChunkData()] BSH data read successfully.");
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::file::BshFile::DecodeTextures()
{
    for (auto offset : m_offsets)
    {
        DecodeTexture(offset);
    }
}

void sg::file::BshFile::DecodeTexture(const uint32_t t_offset)
{
    const auto* offset{ &GetChunk(0).GetData()[t_offset] };

    const auto width{ *reinterpret_cast<const uint32_t*>(offset) };
    const auto height{ *reinterpret_cast<const uint32_t*>((offset += sizeof(uint32_t))) };
    const auto type{ *reinterpret_cast<const uint32_t*>((offset += sizeof(uint32_t))) };
    const auto len{ *reinterpret_cast<const uint32_t*>((offset += sizeof(uint32_t))) };

    if (width <= 0 || height <= 0)
    {
        throw SG_EXCEPTION("[BshFile::DecodeTexture()] Invalid width or height.");
    }

    auto bshTexture{ std::make_unique<BshTexture>() };
    bshTexture->width = width;
    bshTexture->height = height;
    bshTexture->texturePixels.resize(static_cast<size_t>(width) * height);

    auto x{ 0 };
    auto y{ 0 };

    offset += 3;

    while (true)
    {
        const auto numAlpha{ static_cast<uint8_t>(*(offset += sizeof(uint8_t))) };

        if (numAlpha == END_MARKER)
        {
            break;
        }

        if (numAlpha == END_OF_ROW)
        {
            x = 0;
            y++;
            continue;
        }

        for (auto i{ 0 }; i < numAlpha; ++i)
        {
            bshTexture->texturePixels[static_cast<size_t>(y) * width + x] = 0;
            x++;
        }

        const auto numPixels{ static_cast<uint8_t>(*(offset += sizeof(uint8_t))) };

        for (auto i{ 0 }; i < numPixels; ++i)
        {
            const auto colorIndex{ static_cast<uint8_t>(*(offset += sizeof(uint8_t))) };
            bshTexture->texturePixels[static_cast<size_t>(y) * width + x] = m_palette[colorIndex];
            x++;
        }
    }

    m_bshTextures.push_back(std::move(bshTexture));
}

void sg::file::BshFile::CreateGLTextures()
{
    for (const auto& texture : m_bshTextures)
    {
        uint32_t texId;
        glGenTextures(1, &texId);

        glBindTexture(GL_TEXTURE_2D, texId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        texture->textureId = texId;

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            texture->width,
            texture->height,
            0,
            GL_BGRA,
            GL_UNSIGNED_INT_8_8_8_8_REV,
            texture->texturePixels.data()
        );

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
