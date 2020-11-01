#pragma once

#include <string>
#include "BinaryFile.h"

namespace sg::file
{
    class BshFile : public BinaryFile
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        BshFile() = delete;

        explicit BshFile(const std::string& t_filePath);

        BshFile(const BshFile& t_other) = delete;
        BshFile(BshFile&& t_other) noexcept = delete;
        BshFile& operator=(const BshFile& t_other) = delete;
        BshFile& operator=(BshFile&& t_other) noexcept = delete;

        ~BshFile() = default;

        //-------------------------------------------------
        // BinaryFile Interface
        //-------------------------------------------------

        void ReadContentFromChunkData() override;

    protected:

    private:
        static constexpr auto NUMBER_OF_CHUNKS{ 1 };
        inline static const std::string CHUNK_ID{ "BSH" };

        std::vector<uint32_t> m_offsets;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void DecodeTextures();
        void CreateGlTextures();
    };
}
