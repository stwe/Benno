#pragma once

#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include "gl/Shader.h"

namespace sg::camera
{
    class OrthographicCamera;
}

namespace sg::file
{
    class BshFile;
}

namespace sg::renderer
{
    class Zoom;

    class IslandsRenderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        IslandsRenderer() = delete;

        IslandsRenderer(
            std::shared_ptr<file::BshFile> t_bshFile,
            std::vector<glm::mat4>&& t_islandsModelMatrices,
            std::vector<int>&& t_islandsTextureBuffer,
            std::vector<float>&& t_yBuffer,
            std::unordered_map<int, int>&& t_gfxIndexMap
        );

        IslandsRenderer(const IslandsRenderer& t_other) = delete;
        IslandsRenderer(IslandsRenderer&& t_other) noexcept = delete;
        IslandsRenderer& operator=(const IslandsRenderer& t_other) = delete;
        IslandsRenderer& operator=(IslandsRenderer&& t_other) noexcept = delete;

        ~IslandsRenderer();

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init(const Zoom& t_zoom);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Render(const camera::OrthographicCamera& t_camera);

    protected:

    private:
        static constexpr auto DRAW_COUNT{ 6 };
        static constexpr auto MIP_LEVEL_COUNT{ 1 };

        std::shared_ptr<file::BshFile> m_bshFile;
        std::vector<glm::mat4> m_islandsModelMatrices;
        std::vector<int> m_islandsTextureBuffer;
        std::vector<float> m_yBuffer;
        std::unordered_map<int, int> m_gfxIndexMap;

        uint32_t m_vao{ 0 };
        gl::Shader m_shader{ "res/shader/islands/Vertex.vert", "res/shader/islands/Fragment.frag" };
        uint32_t m_textureArrayId{ 0 };

        int m_maxX{ -999 };
        int m_maxY{ -999 };

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

        //-------------------------------------------------
        // Texture array
        //-------------------------------------------------

        void SetMax();
        void CreateTextureArray();
    };
}
