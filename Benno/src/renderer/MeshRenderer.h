#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace sg::gl
{
    class ShaderManager;
}

namespace sg::camera
{
    class OrthographicCamera;
}

namespace sg::renderer
{
    class MeshRenderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit MeshRenderer(std::shared_ptr<gl::ShaderManager> t_shaderManager);

        MeshRenderer(const MeshRenderer& t_other) = delete;
        MeshRenderer(MeshRenderer&& t_other) noexcept = delete;
        MeshRenderer& operator=(const MeshRenderer& t_other) = delete;
        MeshRenderer& operator=(MeshRenderer&& t_other) noexcept = delete;

        ~MeshRenderer() = default;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Render(
            glm::mat4& t_modelMatrix,
            uint32_t t_bshTextureId,
            const camera::OrthographicCamera& t_camera
        ) const;

        void Render(glm::mat4& t_modelMatrix, const camera::OrthographicCamera& t_camera, const glm::vec3& t_color) const;

    protected:

    private:
        std::shared_ptr<gl::ShaderManager> m_shaderManager;

        uint32_t m_vao{ 0 };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        void InitMeshShader() const;
        void InitAabbShader() const;
    };
}
