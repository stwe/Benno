#include <fstream>
#include "Chunk.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::chunk::Chunk::Chunk(std::ifstream& t_input)
{
    t_input.read(m_id.data(), ID_SIZE_IN_BYTES);
    t_input.read(reinterpret_cast<char*>(&m_length), sizeof(m_length));

    if (m_length > 0)
    {
        m_data.resize(m_length);
        t_input.read(reinterpret_cast<char*>(m_data.data()), m_length);
    }
}

sg::chunk::Chunk::~Chunk()
{
}

//-------------------------------------------------
// Getter / read-only
//-------------------------------------------------

const char* sg::chunk::Chunk::GetId() const noexcept
{
    return m_id.c_str();
}

uint32_t sg::chunk::Chunk::GetLength() const noexcept
{
    return m_length;
}

const std::vector<uint8_t>& sg::chunk::Chunk::GetData() const noexcept
{
    return m_data;
}
