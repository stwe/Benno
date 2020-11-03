#pragma once

#include <glm/mat4x4.hpp>
#include "gl/Shader.h"

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
            int t_x,
            int t_y,
            const file::BshTexture& t_bshTexture,
            const glm::mat4& t_projectionMatrix
        );

    protected:

    private:
        uint32_t m_vao{ 0 };
        gl::Shader m_shader{ "res/shader/mesh/Vertex.vert", "res/shader/mesh/Fragment.frag" };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();
    };
}
