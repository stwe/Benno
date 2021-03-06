#pragma once

#include <memory>
#include <string>
#include <vector>

namespace sg::chunk
{
    class Chunk;
}

namespace sg::file
{
    class BinaryFile
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        BinaryFile() = delete;

        explicit BinaryFile(const std::string& t_filePath);

        BinaryFile(const BinaryFile& t_other) = delete;
        BinaryFile(BinaryFile&& t_other) noexcept = delete;
        BinaryFile& operator=(const BinaryFile& t_other) = delete;
        BinaryFile& operator=(BinaryFile&& t_other) noexcept = delete;

        virtual ~BinaryFile();

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] size_t GetNumberOfChunks() const noexcept;
        [[nodiscard]] bool ChunkIndexHasId(int t_chunkIndex, const std::string& t_chunkId) const;
        [[nodiscard]] const chunk::Chunk& GetChunk(int t_chunkIndex) const;
        [[nodiscard]] const std::vector<std::unique_ptr<chunk::Chunk>>& GetChunks() const noexcept;

        //-------------------------------------------------
        // Read Chunks
        //-------------------------------------------------

        void ReadChunksFromFile();

        //-------------------------------------------------
        // Interface
        //-------------------------------------------------

        virtual void ReadContentFromChunkData() = 0;

    protected:

    private:
        std::string m_filePath;
        std::vector<std::unique_ptr<chunk::Chunk>> m_chunks;
    };
}
