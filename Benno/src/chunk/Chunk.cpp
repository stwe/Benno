#include <fstream>
#include <iostream>
#include "Chunk.h"
#include "SgAssert.h"
#include "SgException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::chunk::Chunk::Chunk(std::ifstream& t_input)
{
    t_input.read(m_id.data(), ID_SIZE_IN_BYTES);
    t_input.read(reinterpret_cast<char*>(&m_length), sizeof(m_length));

    SG_ASSERT(m_length > 0, "[Chunk::Chunk()] Invalid Chunk length.");
    SG_ASSERT(t_input.tellg() == CHUNK_SIZE_IN_BYTES, "[Chunk::Chunk()] Invalid file position.");

    m_data.insert(m_data.begin(), std::istreambuf_iterator<char>(t_input), {});

    SG_ASSERT(t_input.tellg() == m_length + CHUNK_SIZE_IN_BYTES, "[Chunk::Chunk()] Invalid file position.");
}

//-------------------------------------------------
// Getter / read-only
//-------------------------------------------------

const std::string& sg::chunk::Chunk::GetId() const noexcept
{
    return m_id;
}

uint32_t sg::chunk::Chunk::GetLength() const noexcept
{
    return m_length;
}

const std::vector<uint8_t>& sg::chunk::Chunk::GetData() const noexcept
{
    return m_data;
}
