#pragma once

#include <glm/mat4x4.hpp>
#include "BinaryFile.h"

namespace sg::renderer
{
    class MeshRenderer;
}

namespace sg::chunk
{
    class Island5;
}

namespace sg::data
{
    class HousesJsonFile;
}

namespace sg::file
{
    class BshFile;

    class GamFile : public BinaryFile
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        GamFile() = delete;

        GamFile(const std::string& t_filePath, std::shared_ptr<data::HousesJsonFile> t_housesJsonFile);

        GamFile(const GamFile& t_other) = delete;
        GamFile(GamFile&& t_other) noexcept = delete;
        GamFile& operator=(const GamFile& t_other) = delete;
        GamFile& operator=(GamFile&& t_other) noexcept = delete;

        ~GamFile();

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] const std::vector<std::unique_ptr<chunk::Island5>>& GetIsland5List() const noexcept;

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(BshFile* t_bshFile, renderer::MeshRenderer* t_renderer, const glm::mat4& t_mat) const;

        //-------------------------------------------------
        // BinaryFile Interface
        //-------------------------------------------------

        void ReadContentFromChunkData() override;

    protected:

    private:
        std::shared_ptr<data::HousesJsonFile> m_housesJsonFile;

        std::vector<std::unique_ptr<chunk::Island5>> m_island5List;

        //-------------------------------------------------
        // Island5 Layer
        //-------------------------------------------------

        void InitIsland5Layer();
    };
}
