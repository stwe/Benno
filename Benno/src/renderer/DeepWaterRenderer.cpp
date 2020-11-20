#include <glm/vec4.hpp>
#include "DeepWaterRenderer.h"
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

sg::renderer::DeepWaterRenderer::DeepWaterRenderer(
    std::shared_ptr<file::BshFile> t_bshFile,
    std::vector<glm::mat4>&& t_deepWaterModelMatrices,
    std::vector<int>&& t_deepWaterTextureBuffer
)
    : m_bshFile{ std::move(t_bshFile) }
    , m_deepWaterModelMatrices{ t_deepWaterModelMatrices }
    , m_deepWaterTextureBuffer{ t_deepWaterTextureBuffer }
{
    Log::SG_LOG_DEBUG("[DeepWaterRenderer::DeepWaterRenderer()] Create DeepWaterRenderer.");
}

sg::renderer::DeepWaterRenderer::~DeepWaterRenderer()
{
    Log::SG_LOG_DEBUG("[DeepWaterRenderer::~DeepWaterRenderer()] Destruct DeepWaterRenderer.");
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::renderer::DeepWaterRenderer::Init(const Zoom& t_zoom)
{
    m_shader.AddUniform("viewProjection");
    m_shader.AddUniform("sampler");

    m_textureWidth = t_zoom.GetDefaultTileWidth();
    m_textureHeight = t_zoom.GetDefaultTileHeight();

    CreateMesh();
    AddModelMatricesVbo();
    AddTextureIndexVbo();
    CreateTextureArray();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::renderer::DeepWaterRenderer::Render(const camera::OrthographicCamera& t_camera)
{
    OpenGL::EnableAlphaBlending();

    m_shader.Bind();
    glBindVertexArray(m_vao);

    gl::Texture::BindForReading(m_textureArrayId, GL_TEXTURE0, GL_TEXTURE_2D_ARRAY);

    m_shader.SetUniform("viewProjection", t_camera.GetViewProjectionMatrix());
    m_shader.SetUniform("sampler", 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, DRAW_COUNT, static_cast<uint32_t>(m_deepWaterModelMatrices.size()));

    glBindVertexArray(0);
    gl::Shader::Unbind();

    OpenGL::DisableBlending();
}

//-------------------------------------------------
// Mesh
//-------------------------------------------------

void sg::renderer::DeepWaterRenderer::CreateMesh()
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
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind VAO
    glBindVertexArray(0);
}

//-------------------------------------------------
// Vbo
//-------------------------------------------------

void sg::renderer::DeepWaterRenderer::AddModelMatricesVbo()
{
    // bind VAO
    glBindVertexArray(m_vao);

    // create VBO
    uint32_t vbo;
    glGenBuffers(1, &vbo);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // stora data
    glBufferData(GL_ARRAY_BUFFER, m_deepWaterModelMatrices.size() * sizeof glm::mat4, m_deepWaterModelMatrices.data(), GL_STATIC_DRAW);

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

void sg::renderer::DeepWaterRenderer::AddTextureIndexVbo()
{
    // bind VAO
    glBindVertexArray(m_vao);

    // create VBO
    uint32_t vbo;
    glGenBuffers(1, &vbo);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // store data
    glBufferData(GL_ARRAY_BUFFER, m_deepWaterTextureBuffer.size() * sizeof(int), m_deepWaterTextureBuffer.data(), GL_STATIC_DRAW);

    // set layout
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, 4, nullptr);

    glVertexAttribDivisor(5, 1);

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind VAO
    glBindVertexArray(0);
}

//-------------------------------------------------
// Texture array
//-------------------------------------------------

void sg::renderer::DeepWaterRenderer::CreateTextureArray()
{
    m_textureArrayId = gl::Texture::GenerateNewTextureId();
    gl::Texture::Bind(m_textureArrayId, GL_TEXTURE_2D_ARRAY);
    glTextureStorage3D(m_textureArrayId, MIP_LEVEL_COUNT, GL_RGBA8, m_textureWidth, m_textureHeight, LAYER_COUNT);

    auto zOffset{ 0 };

    for (auto i{ START_GFX_INDEX }; i <= END_GFX_INDEX; ++i)
    {
        const auto& currentTexture{ m_bshFile->GetBshTexture(i) };
        if (currentTexture.width != m_textureWidth ||
            currentTexture.height != m_textureHeight) {
            throw SG_EXCEPTION("[DeepWaterRenderer::CreateTextureArray()] Invalid texture size.");
        }

        glTextureSubImage3D(
            m_textureArrayId,
            0,
            0, 0,
            zOffset,
            m_textureWidth, m_textureHeight,
            1,
            GL_BGRA,
            GL_UNSIGNED_INT_8_8_8_8_REV,
            currentTexture.texturePixels.data()
        );

        zOffset++;
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
