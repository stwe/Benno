#pragma once

#include "BinaryFile.h"

namespace sg::chunk
{
    class Island5;
}

namespace sg::file
{
    class GamFile : public BinaryFile
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        GamFile() = delete;

        explicit GamFile(const std::string& t_filePath);

        GamFile(const GamFile& t_other) = delete;
        GamFile(GamFile&& t_other) noexcept = delete;
        GamFile& operator=(const GamFile& t_other) = delete;
        GamFile& operator=(GamFile&& t_other) noexcept = delete;

        ~GamFile();

        //-------------------------------------------------
        // BinaryFile Interface
        //-------------------------------------------------

        void ReadContentFromChunkData() override;

    protected:

    private:
        std::vector<std::unique_ptr<chunk::Island5>> m_island5List;
    };
}
