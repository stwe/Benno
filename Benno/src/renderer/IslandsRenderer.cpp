#include <glm/vec4.hpp>
#include "IslandsRenderer.h"
#include "Log.h"
#include "OpenGL.h"
#include "Zoom.h"
#include "gl/Texture.h"
#include "file/BshFile.h"
#include "file/BshTexture.h"
#include "camera/OrthographicCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::renderer::IslandsRenderer::IslandsRenderer(
    std::shared_ptr<file::BshFile> t_bshFile,
    std::vector<glm::mat4>&& t_islandsModelMatrices,
    std::vector<int>&& t_islandsTextureBuffer,
    std::vector<float>&& t_yBuffer,
    std::unordered_map<int, int>&& t_gfxIndexMap
)
    : m_bshFile{ std::move(t_bshFile) }
    , m_islandsModelMatrices{ std::move(t_islandsModelMatrices) }
    , m_islandsTextureBuffer{ std::move(t_islandsTextureBuffer) }
    , m_yBuffer{ std::move(t_yBuffer) }
    , m_gfxIndexMap{ std::move(t_gfxIndexMap) }
{
    Log::SG_LOG_DEBUG("[IslandsRenderer::IslandsRenderer()] Create IslandsRenderer.");
}

sg::renderer::IslandsRenderer::~IslandsRenderer()
{
    Log::SG_LOG_DEBUG("[IslandsRenderer::~IslandsRenderer()] Destruct IslandsRenderer.");
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::renderer::IslandsRenderer::Init(const Zoom& t_zoom)
{
    m_shader.AddUniform("viewProjection");
    m_shader.AddUniform("sampler");
    m_shader.AddUniform("maxY");

    CreateMesh();

    AddModelMatricesVbo();
    AddTextureIndexVbo();
    AddYVbo();

    SetMax();
    CreateTextureArray();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::renderer::IslandsRenderer::Render(const camera::OrthographicCamera& t_camera)
{
    OpenGL::EnableAlphaBlending();

    m_shader.Bind();
    glBindVertexArray(m_vao);

    gl::Texture::BindForReading(m_textureArrayId, GL_TEXTURE0, GL_TEXTURE_2D_ARRAY);

    m_shader.SetUniform("viewProjection", t_camera.GetViewProjectionMatrix());
    m_shader.SetUniform("sampler", 0);
    m_shader.SetUniform("maxY", static_cast<float>(m_maxY));

    glDrawArraysInstanced(GL_TRIANGLES, 0, DRAW_COUNT, static_cast<uint32_t>(m_islandsModelMatrices.size()));

    glBindVertexArray(0);
    gl::Shader::Unbind();

    OpenGL::DisableBlending();
}

//-------------------------------------------------
// Mesh
//-------------------------------------------------

void sg::renderer::IslandsRenderer::CreateMesh()
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

    // create VAO
    glGenVertexArrays(1, &m_vao);

    // bind VAO
    glBindVertexArray(m_vao);

    // create VBO
    uint32_t vbo;
    glGenBuffers(1, &vbo);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // store data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set buffer layout
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), (void*)0);

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind VAO
    glBindVertexArray(0);
}

//-------------------------------------------------
// Vbo
//-------------------------------------------------

void sg::renderer::IslandsRenderer::AddModelMatricesVbo()
{
    // bind VAO
    glBindVertexArray(m_vao);

    // create VBO
    uint32_t vbo;
    glGenBuffers(1, &vbo);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // stora data
    glBufferData(GL_ARRAY_BUFFER, m_islandsModelMatrices.size() * sizeof glm::mat4, m_islandsModelMatrices.data(), GL_STATIC_DRAW);

    // set buffer layout
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, 4 * sizeof(glm::vec4), (void*)0);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, false, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, false, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, false, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind VAO
    glBindVertexArray(0);
}

void sg::renderer::IslandsRenderer::AddTextureIndexVbo()
{
    // bind VAO
    glBindVertexArray(m_vao);

    // create VBO
    uint32_t vbo;
    glGenBuffers(1, &vbo);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // store data
    glBufferData(GL_ARRAY_BUFFER, m_islandsTextureBuffer.size() * sizeof(int), m_islandsTextureBuffer.data(), GL_STATIC_DRAW);

    // set layout
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, 4, nullptr);

    glVertexAttribDivisor(5, 1);

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind VAO
    glBindVertexArray(0);
}

void sg::renderer::IslandsRenderer::AddYVbo()
{
    // bind VAO
    glBindVertexArray(m_vao);

    // create VBO
    uint32_t vbo;
    glGenBuffers(1, &vbo);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // store data
    glBufferData(GL_ARRAY_BUFFER, m_yBuffer.size() * sizeof(float), m_yBuffer.data(), GL_STATIC_DRAW);

    // set layout
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, false, sizeof(float), (void*)0);

    glVertexAttribDivisor(6, 1);

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind VAO
    glBindVertexArray(0);
}

//-------------------------------------------------
// Texture array
//-------------------------------------------------

void sg::renderer::IslandsRenderer::SetMax()
{
    for (const auto& texture : m_bshFile->GetBshTextures())
    {
        if (texture->width > m_maxX)
        {
            m_maxX = texture->width;
        }

        if (texture->height > m_maxY)
        {
            m_maxY = texture->height;
        }
    }
}

void sg::renderer::IslandsRenderer::CreateTextureArray()
{
    m_textureArrayId = gl::Texture::GenerateNewTextureId();
    gl::Texture::Bind(m_textureArrayId, GL_TEXTURE_2D_ARRAY);
    glTextureStorage3D(m_textureArrayId, MIP_LEVEL_COUNT, GL_RGBA8, m_maxX, m_maxY, static_cast<int32_t>(m_gfxIndexMap.size()));

    std::vector<int> empty(static_cast<size_t>(m_maxX) * m_maxY, 0);

    for (const auto& entry : m_gfxIndexMap)
    {
        // key gfx : value pos

        const auto& currentTexture{ m_bshFile->GetBshTexture(entry.first) };

        glTextureSubImage3D(
            m_textureArrayId,
            0,
            0, 0,
            entry.second, // zOffset
            m_maxX, m_maxY,
            1,
            GL_BGRA,
            GL_UNSIGNED_INT_8_8_8_8_REV,
            empty.data()
        );

        glTextureSubImage3D(
            m_textureArrayId,
            0,
            0, 0,
            entry.second,
            currentTexture.width, currentTexture.height,
            1,
            GL_BGRA,
            GL_UNSIGNED_INT_8_8_8_8_REV,
            currentTexture.texturePixels.data()
        );
    }
}
