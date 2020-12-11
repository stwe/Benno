#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

namespace sg::gl
{
    class ShaderManager;
}

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
            std::shared_ptr<gl::ShaderManager> t_shaderManager,
            std::shared_ptr<file::BshFile> t_bshFile,
            std::vector<glm::mat4>&& t_deepWaterModelMatrices,
            std::vector<int>&& t_deepWaterTextureBuffer,
            std::vector<glm::vec3>&& t_intensityBuffer
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

        void Render(const camera::OrthographicCamera& t_camera, bool t_wireframe = false);

        //-------------------------------------------------
        // Update Vbo
        //-------------------------------------------------

        void UpdateIntensity(int t_index, const glm::vec3& t_intensity) const;

    protected:

    private:
        static constexpr auto DRAW_COUNT{ 6 };
        static constexpr auto MIP_LEVEL_COUNT{ 1 };
        static constexpr auto LAYER_COUNT{ 12 };
        static constexpr auto END_GFX_INDEX{ 769 };

        std::shared_ptr<gl::ShaderManager> m_shaderManager;
        std::shared_ptr<file::BshFile> m_bshFile;
        std::vector<glm::mat4> m_deepWaterModelMatrices;
        std::vector<int> m_deepWaterTextureBuffer;
        std::vector<glm::vec3> m_intensityBuffer;

        uint32_t m_vaoId{ 0 };
        uint32_t m_textureArrayId{ 0 };
        uint32_t m_instances{ 0 };
        uint32_t m_intensityVboId{ 0 };

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
        void AddIntensityVbo();

        //-------------------------------------------------
        // Texture array
        //-------------------------------------------------

        void CreateTextureArray();
    };
}
