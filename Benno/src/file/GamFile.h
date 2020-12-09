#pragma once

#include "BinaryFile.h"
#include "renderer/Zoom.h"
#include "chunk/TileGraphic.h"

namespace sg::camera
{
    class OrthographicCamera;
}

namespace sg::renderer
{
    class DeepWaterRenderer;
    class IslandModel;
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

        GamFile(
            const std::string& t_filePath,
            std::shared_ptr<BshFile> t_bshFile,
            std::shared_ptr<data::HousesJsonFile> t_housesJsonFile,
            const renderer::Zoom& t_zoom
        );

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

        void Render(
            camera::OrthographicCamera& t_camera,
            int& t_info,
            bool t_renderIslandAabbs = false
        );

        void Update(int t_mapX, int t_mapY);

        //-------------------------------------------------
        // BinaryFile Interface
        //-------------------------------------------------

        void ReadContentFromChunkData() override;

    protected:

    private:
        static constexpr auto DARK{ glm::vec3(0.4f) };
        static constexpr auto BRIGHT{ glm::vec3(1.0f) };
        static constexpr auto NO_DEEP_WATER{ -1 };

        std::shared_ptr<BshFile> m_bshFile;
        std::shared_ptr<data::HousesJsonFile> m_housesJsonFile;
        renderer::Zoom m_zoom;

        std::vector<std::unique_ptr<chunk::Island5>> m_island5List;

        std::unique_ptr<renderer::DeepWaterRenderer> m_deepWaterRenderer;
        std::unique_ptr<renderer::MeshRenderer> m_meshRenderer;

        std::vector<std::unique_ptr<renderer::IslandModel>> m_islandModels;

        std::vector<int> m_deepWaterIndex;

        //-------------------------------------------------
        // Island5 Layer
        //-------------------------------------------------

        void InitIsland5Layer();

        //-------------------------------------------------
        // Deep water area
        //-------------------------------------------------

        void InitDeepWaterArea();
        void CreateDeepWaterGraphicTiles(std::vector<chunk::TileGraphic>& t_graphicTiles);
        int DeepWaterTileInVbo(int t_mapX, int t_mapY);

        //-------------------------------------------------
        // Islands area
        //-------------------------------------------------

        void InitIslandsArea();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static chunk::Island5* IsIslandOnPosition(int t_x, int t_y, const std::vector<std::unique_ptr<chunk::Island5>>& t_island5List);
    };
}
