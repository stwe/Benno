#pragma once

#include <exception>
#include <string>

namespace sg
{
#if defined(_WIN64) && defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable: 4275)
#endif
    class SgException : public std::exception
    {
#if defined(_WIN64) && defined(_MSC_VER)
    #pragma warning(pop)
#endif
    public:
        SgException(int t_line, const char* t_file, std::string t_message);

        const char* what() const noexcept override;

        virtual const char* GetType() const noexcept;

        int GetLine() const noexcept;
        const std::string& GetFile() const noexcept;
        std::string GetOriginString() const noexcept;

    protected:
        mutable std::string m_whatBuffer;

    private:
        int m_line;
        std::string m_file;
        std::string m_message;
    };
}

#define SG_EXCEPTION(message) sg::SgException(__LINE__, __FILE__, message)
