#include <glm/gtc/matrix_transform.hpp>
#include "MeshRenderer.h"
#include "OpenGL.h"
#include "file/BshTexture.h"
#include "gl/Texture.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::renderer::MeshRenderer::MeshRenderer()
{
    Init();

    m_shader.AddUniform("model");
    m_shader.AddUniform("projection");
    m_shader.AddUniform("diffuseMap");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::renderer::MeshRenderer::Render(
    const float t_x,
    const float t_y,
    const file::BshTexture& t_bshTexture,
    const glm::mat4& t_projectionMatrix
)
{
    glBindVertexArray(m_vao);
    m_shader.Bind();

    auto model = glm::mat4(1.0f);
    model = translate(model, glm::vec3(t_x, t_y, 0.0f));
    model = scale(model, glm::vec3(t_bshTexture.width, t_bshTexture.height, 1.0f));

    gl::Texture::BindForReading(t_bshTexture.textureId, GL_TEXTURE0);

    m_shader.SetUniform("model", model);
    m_shader.SetUniform("projection", t_projectionMatrix);
    m_shader.SetUniform("diffuseMap", 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    gl::Shader::Unbind();
    glBindVertexArray(0);
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
