#include <glm/gtc/type_ptr.hpp>
#include "DeepWaterRenderer.h"
#include "Log.h"
#include "OpenGL.h"
#include "Zoom.h"
#include "gl/Texture.h"
#include "file/BshFile.h"
#include "file/BshTexture.h"
#include "chunk/TileUtil.h"
#include "camera/OrthographicCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::renderer::DeepWaterRenderer::DeepWaterRenderer(
    std::shared_ptr<file::BshFile> t_bshFile,
    std::vector<glm::mat4>&& t_deepWaterModelMatrices,
    std::vector<int>&& t_deepWaterTextureBuffer,
    std::vector<glm::vec3>&& t_intensityBuffer
)
    : m_bshFile{ std::move(t_bshFile) }
    , m_deepWaterModelMatrices{ std::move(t_deepWaterModelMatrices) }
    , m_deepWaterTextureBuffer{ std::move(t_deepWaterTextureBuffer) }
    , m_intensityBuffer{ std::move(t_intensityBuffer) }
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
    AddIntensityVbo();

    CreateTextureArray();

    m_instances = static_cast<uint32_t>(m_deepWaterModelMatrices.size());
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::renderer::DeepWaterRenderer::Render(const camera::OrthographicCamera& t_camera, const bool t_wireframe)
{
    !t_wireframe ? OpenGL::EnableAlphaBlending() : OpenGL::EnableWireframeMode();

    m_shader.Bind();
    glBindVertexArray(m_vaoId);

    gl::Texture::BindForReading(m_textureArrayId, GL_TEXTURE0, GL_TEXTURE_2D_ARRAY);

    m_shader.SetUniform("viewProjection", t_camera.GetViewProjectionMatrix());
    m_shader.SetUniform("sampler", 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, DRAW_COUNT, m_instances);

    glBindVertexArray(0);
    gl::Shader::Unbind();

    !t_wireframe ? OpenGL::DisableBlending() : OpenGL::DisableWireframeMode();
}

//-------------------------------------------------
// Update Vbo
//-------------------------------------------------

void sg::renderer::DeepWaterRenderer::UpdateIntensity(const int t_mapX, const int t_mapY, const glm::vec3& t_intensity) const
{
    if (t_mapX < 0 || t_mapY < 0)
    {
        return;
    }

    // todo: Denkfehler:
    // todo: das kann so nicht funktionieren, da zwar der Index richtig berechnet wird, aber
    // todo: nur bezogen auf alle Tiles; der InsensityVbo beinhaltet nur DeepWaterTiles und deckt
    // todo: somit nicht die ganze Karte ab (Index -> fehlender Value)

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_intensityVboId);

    // update intensity at the specified index
    glBufferSubData(
        GL_ARRAY_BUFFER,
        chunk::TileUtil::GetIndexFrom2D(t_mapX, t_mapY) * sizeof(glm::vec3),
        sizeof(glm::vec3),
        glm::value_ptr(t_intensity)
    );

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    glGenVertexArrays(1, &m_vaoId);

    // bind VAO
    glBindVertexArray(m_vaoId);

    // create VBO
    uint32_t vboId;
    glGenBuffers(1, &vboId);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    // store data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set buffer layout
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

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
    glBindVertexArray(m_vaoId);

    // create VBO
    uint32_t vboId;
    glGenBuffers(1, &vboId);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    // stora data
    glBufferData(GL_ARRAY_BUFFER, m_deepWaterModelMatrices.size() * sizeof(glm::mat4), m_deepWaterModelMatrices.data(), GL_STATIC_DRAW);

    // set buffer layout
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), nullptr);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

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
    glBindVertexArray(m_vaoId);

    // create VBO
    uint32_t vboId;
    glGenBuffers(1, &vboId);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    // store data
    glBufferData(GL_ARRAY_BUFFER, m_deepWaterTextureBuffer.size() * sizeof(int), m_deepWaterTextureBuffer.data(), GL_STATIC_DRAW);

    // set layout
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 1, GL_INT, sizeof(int), nullptr);

    glVertexAttribDivisor(5, 1);

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind VAO
    glBindVertexArray(0);
}

void sg::renderer::DeepWaterRenderer::AddIntensityVbo()
{
    // bind VAO
    glBindVertexArray(m_vaoId);

    // create VBO
    glGenBuffers(1, &m_intensityVboId);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_intensityVboId);

    // store data
    glBufferData(GL_ARRAY_BUFFER, m_intensityBuffer.size() * sizeof(glm::vec3), m_intensityBuffer.data(), GL_DYNAMIC_DRAW);

    // set layout
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glVertexAttribDivisor(6, 1);

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
