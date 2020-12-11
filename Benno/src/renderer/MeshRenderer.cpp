#include <glm/gtc/matrix_transform.hpp>
#include "MeshRenderer.h"
#include "OpenGL.h"
#include "gl/Texture.h"
#include "gl/Shader.h"
#include "gl/ShaderManager.h"
#include "camera/OrthographicCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::renderer::MeshRenderer::MeshRenderer(std::shared_ptr<gl::ShaderManager> t_shaderManager)
    : m_shaderManager(std::move(t_shaderManager))
{
    Init();

    InitMeshShader();
    InitAabbShader();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::renderer::MeshRenderer::Render(
    glm::mat4& t_modelMatrix,
    const uint32_t t_bshTextureId,
    const camera::OrthographicCamera& t_camera
) const
{
    OpenGL::EnableAlphaBlending();

    auto& shader{ m_shaderManager->GetShader("mesh") };
    shader.Bind();
    glBindVertexArray(m_vao);

    gl::Texture::BindForReading(t_bshTextureId, GL_TEXTURE0);

    shader.SetUniform("model", t_modelMatrix);
    shader.SetUniform("viewProjection", t_camera.GetViewProjectionMatrix());
    shader.SetUniform("diffuseMap", 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    gl::Shader::Unbind();

    OpenGL::DisableBlending();
}

void sg::renderer::MeshRenderer::Render(glm::mat4& t_modelMatrix, const camera::OrthographicCamera& t_camera, const glm::vec3& t_color) const
{
    OpenGL::EnableAlphaBlending();
    OpenGL::EnableWireframeMode();

    auto& shader{ m_shaderManager->GetShader("aabb") };
    shader.Bind();
    glBindVertexArray(m_vao);

    shader.SetUniform("model", t_modelMatrix);
    shader.SetUniform("viewProjection", t_camera.GetViewProjectionMatrix());
    shader.SetUniform("color", t_color);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    gl::Shader::Unbind();

    OpenGL::DisableWireframeMode();
    OpenGL::DisableBlending();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

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
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void sg::renderer::MeshRenderer::InitMeshShader() const
{
    auto& shader{ m_shaderManager->GetShader("mesh") };
    shader.AddUniform("model");
    shader.AddUniform("viewProjection");
    shader.AddUniform("diffuseMap");
}

void sg::renderer::MeshRenderer::InitAabbShader() const
{
    auto& shader{ m_shaderManager->GetShader("aabb") };
    shader.AddUniform("model");
    shader.AddUniform("viewProjection");
    shader.AddUniform("color");
}
