#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "Zoom.h"
#include "chunk/TileGraphic.h"
#include "physics/Aabb.h"
#include "gl/Shader.h"

namespace sg::camera
{
    class OrthographicCamera;
}

namespace sg::chunk
{
    class Island5;
}

namespace sg::file
{
    class BshFile;
}

namespace sg::renderer
{
    class IslandModel
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        IslandModel(Zoom& t_zoom, chunk::Island5* t_parentIsland, std::shared_ptr<file::BshFile> t_bshFile);

        IslandModel(const IslandModel& t_other) = delete;
        IslandModel(IslandModel&& t_other) noexcept = delete;
        IslandModel& operator=(const IslandModel& t_other) = delete;
        IslandModel& operator=(IslandModel&& t_other) noexcept = delete;

        ~IslandModel();

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(const camera::OrthographicCamera& t_camera);

        //-------------------------------------------------
        // Update Vbo
        //-------------------------------------------------

        void UpdateIntensity(int t_index, const glm::vec3& t_intensity) const;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] chunk::Island5* GetParentIsland() const noexcept;
        [[nodiscard]] physics::Aabb GetAabb() const noexcept;
        [[nodiscard]] const std::vector<int>& GetIslandIndex() const noexcept;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        int IslandTileInVbo(int t_mapX, int t_mapY);

    protected:

    private:
        static constexpr auto DRAW_COUNT{ 6 };
        static constexpr auto MIP_LEVEL_COUNT{ 1 };
        static constexpr auto MAX_TEXTURES{ 1000 };

        static constexpr auto DARK{ glm::vec3(0.1f) };
        static constexpr auto NO_ISLAND{ -1 };

        Zoom m_zoom;
        chunk::Island5* m_parentIsland{ nullptr };
        std::shared_ptr<file::BshFile> m_bshFile;

        std::vector<chunk::TileGraphic> m_graphicTiles;
        std::vector<glm::mat4> m_modelMatrices;
        std::vector<int> m_textureBuffer;
        std::vector<float> m_yBuffer;
        std::unordered_map<int, int> m_gfxIndexMap;
        std::vector<glm::vec3> m_intensityBuffer;

        uint32_t m_vaoId{ 0 };
        gl::Shader m_shader{ "res/shader/islands/Vertex.vert", "res/shader/islands/Fragment.frag" };
        uint32_t m_textureArrayId{ 0 };
        uint32_t m_instances{ 0 };
        uint32_t m_intensityVboId{ 0 };

        std::vector<int> m_islandIndex;

        physics::Aabb m_aabb;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // Tile data
        //-------------------------------------------------

        void CreateGraphicTiles();
        void CreateModelMatrices();
        void CreateTextureIndex();

        //-------------------------------------------------
        // Physics
        //-------------------------------------------------

        void CreateAabb();

        //-------------------------------------------------
        // Shader
        //-------------------------------------------------

        void InitShader();

        //-------------------------------------------------
        // Mesh
        //-------------------------------------------------

        void CreateMesh();

        //-------------------------------------------------
        // Vbo
        //-------------------------------------------------

        void AddModelMatricesVbo();
        void AddTextureIndexVbo();
        void AddYVbo();
        void AddIntensityVbo();

        //-------------------------------------------------
        // Texture array
        //-------------------------------------------------

        void CreateTextureArray();
    };
}
