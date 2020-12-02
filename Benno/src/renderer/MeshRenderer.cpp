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

    m_meshShader.AddUniform("model");
    m_meshShader.AddUniform("viewProjection");
    m_meshShader.AddUniform("diffuseMap");

    m_aabbShader.AddUniform("model");
    m_aabbShader.AddUniform("viewProjection");
    m_aabbShader.AddUniform("color");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::renderer::MeshRenderer::Render(
    glm::mat4& t_modelMatrix,
    const uint32_t t_bshTextureId,
    const camera::OrthographicCamera& t_camera
)
{
    OpenGL::EnableAlphaBlending();

    m_meshShader.Bind();
    glBindVertexArray(m_vao);

    gl::Texture::BindForReading(t_bshTextureId, GL_TEXTURE0);

    m_meshShader.SetUniform("model", t_modelMatrix);
    m_meshShader.SetUniform("viewProjection", t_camera.GetViewProjectionMatrix());
    m_meshShader.SetUniform("diffuseMap", 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    gl::Shader::Unbind();

    OpenGL::DisableBlending();
}

void sg::renderer::MeshRenderer::Render(glm::mat4& t_modelMatrix, const camera::OrthographicCamera& t_camera, const glm::vec3& t_color)
{
    OpenGL::EnableAlphaBlending();
    OpenGL::EnableWireframeMode();

    m_aabbShader.Bind();
    glBindVertexArray(m_vao);

    m_aabbShader.SetUniform("model", t_modelMatrix);
    m_aabbShader.SetUniform("viewProjection", t_camera.GetViewProjectionMatrix());
    m_aabbShader.SetUniform("color", t_color);

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
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
