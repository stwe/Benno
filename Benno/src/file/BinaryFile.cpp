#include <fstream>
#include "BinaryFile.h"
#include "SgException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::file::BinaryFile::BinaryFile(const std::string& t_filePath)
    : m_filePath{ t_filePath }
{
    ReadChunksFromFile();
}

//-------------------------------------------------
// Getter / read-only
//-------------------------------------------------

size_t sg::file::BinaryFile::GetNumberOfChunks() const noexcept
{
    return m_chunks.size();
}

bool sg::file::BinaryFile::ChunkIndexHasId(const int t_chunkIndex, const std::string& t_chunkId) const
{
    return m_chunks.at(t_chunkIndex)->GetId() == t_chunkId;
}

const sg::chunk::Chunk& sg::file::BinaryFile::GetChunk(const int t_chunkIndex) const
{
    return *m_chunks.at(t_chunkIndex);
}

//-------------------------------------------------
// Read Chunks
//-------------------------------------------------

void sg::file::BinaryFile::ReadChunksFromFile()
{
    std::ifstream input(m_filePath, std::ios::binary);

    if (!input.is_open())
    {
        throw SG_EXCEPTION("[BinaryFile::ReadChunksFromFile()] Error while opening file " + m_filePath + ".");
    }

    while (input.peek() != EOF)
    {
        m_chunks.push_back(std::make_unique<chunk::Chunk>(input));
    }

    input.close();
}
