#pragma once

#include <memory>
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

    class DeepWaterRenderer
    {
    public:
        static constexpr auto START_GFX_INDEX{ 758 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        DeepWaterRenderer() = delete;

        DeepWaterRenderer(
            std::shared_ptr<file::BshFile> t_bshFile,
            std::vector<glm::mat4>&& t_deepWaterModelMatrices,
            std::vector<int>&& t_deepWaterTextureBuffer
        );

        DeepWaterRenderer(const DeepWaterRenderer& t_other) = delete;
        DeepWaterRenderer(DeepWaterRenderer&& t_other) noexcept = delete;
        DeepWaterRenderer& operator=(const DeepWaterRenderer& t_other) = delete;
        DeepWaterRenderer& operator=(DeepWaterRenderer&& t_other) noexcept = delete;

        ~DeepWaterRenderer();

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
        static constexpr auto LAYER_COUNT{ 12 };
        static constexpr auto END_GFX_INDEX{ 769 };

        std::shared_ptr<file::BshFile> m_bshFile;
        std::vector<glm::mat4> m_deepWaterModelMatrices;
        std::vector<int> m_deepWaterTextureBuffer;

        uint32_t m_vao{ 0 };
        gl::Shader m_shader{ "res/shader/deepWater/Vertex.vert", "res/shader/deepWater/Fragment.frag" };
        uint32_t m_textureArrayId{ 0 };

        int m_textureWidth{ 0 };
        int m_textureHeight{ 0 };

        //-------------------------------------------------
        // Mesh
        //-------------------------------------------------

        void CreateMesh();

        //-------------------------------------------------
        // Vbo
        //-------------------------------------------------

        void AddModelMatricesVbo();
        void AddTextureIndexVbo();

        //-------------------------------------------------
        // Texture array
        //-------------------------------------------------

        void CreateTextureArray();
    };
}
