#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "gl/Shader.h"

namespace sg::camera
{
    class OrthographicCamera;
}

namespace sg::file
{
    struct BshTexture;
}

namespace sg::renderer
{
    class MeshRenderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MeshRenderer();

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
        );

        void Render(glm::mat4& t_modelMatrix, const camera::OrthographicCamera& t_camera, const glm::vec3& t_color);

    protected:

    private:
        uint32_t m_vao{ 0 };

        gl::Shader m_meshShader{ "res/shader/mesh/Vertex.vert", "res/shader/mesh/Fragment.frag" };
        gl::Shader m_aabbShader{ "res/shader/aabb/Vertex.vert", "res/shader/aabb/Fragment.frag" };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();
    };
}
