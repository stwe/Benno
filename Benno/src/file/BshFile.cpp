#include "BshFile.h"
#include "Log.h"
#include "SgException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::file::BshFile::BshFile(const std::string& t_filePath)
    : BinaryFile(t_filePath)
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
    CreateGlTextures();

    Log::SG_LOG_DEBUG("[BshFile::ReadContentFromChunkData()] BSH data read successfully.");
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::file::BshFile::DecodeTextures()
{
}

void sg::file::BshFile::CreateGlTextures()
{
}
