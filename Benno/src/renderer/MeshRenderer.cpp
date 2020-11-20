#include <glm/gtc/matrix_transform.hpp>
#include "MeshRenderer.h"
#include "OpenGL.h"
#include "file/BshTexture.h"
#include "gl/Texture.h"
#include "camera/OrthographicCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::renderer::MeshRenderer::MeshRenderer()
{
    Init();

    m_shader.AddUniform("model");
    m_shader.AddUniform("viewProjection");
    m_shader.AddUniform("diffuseMap");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::renderer::MeshRenderer::Render(
    const float t_x,
    const float t_y,
    const file::BshTexture& t_bshTexture,
    const camera::OrthographicCamera& t_camera
)
{
    OpenGL::EnableAlphaBlending();

    m_shader.Bind();
    glBindVertexArray(m_vao);

    auto model = glm::mat4(1.0f);
    model = translate(model, glm::vec3(t_x, t_y, 0.0f));
    model = scale(model, glm::vec3(t_bshTexture.width, t_bshTexture.height, 1.0f));

    gl::Texture::BindForReading(t_bshTexture.textureId, GL_TEXTURE0);

    m_shader.SetUniform("model", model);
    m_shader.SetUniform("viewProjection", t_camera.GetViewProjectionMatrix());
    m_shader.SetUniform("diffuseMap", 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    gl::Shader::Unbind();

    OpenGL::DisableBlending();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

// todo Vao && Vbo classes
void sg::renderer::MeshRenderer::Init()
{
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &m_vao);

    uint32_t vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
