#include <sstream>
#include "SgException.h"

sg::SgException::SgException(const int t_line, const char* t_file, std::string t_message)
    : m_line{ t_line }
    , m_file{ t_file }
    , m_message{ std::move(t_message) }
{
}

const char* sg::SgException::what() const noexcept
{
    std::ostringstream oss;

    oss << "occurred: \n";
    oss << "[Type]: " << GetType() << "\n";
    oss << GetOriginString();

    m_whatBuffer = oss.str();

    return m_whatBuffer.c_str();
}

const char* sg::SgException::GetType() const noexcept
{
    return "SgException";
}

int sg::SgException::GetLine() const noexcept
{
    return m_line;
}

const std::string& sg::SgException::GetFile() const noexcept
{
    return m_file;
}

std::string sg::SgException::GetOriginString() const noexcept
{
    std::ostringstream oss;

    oss << "[Message]: " << m_message << "\n";
    oss << "[File]: " << m_file << "\n";
    oss << "[Line]: " << m_line;

    return oss.str();
}
