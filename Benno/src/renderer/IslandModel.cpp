#include <unordered_set>
#include "IslandModel.h"
#include "OpenGL.h"
#include "SgAssert.h"
#include "Zoom.h"
#include "Log.h"
#include "chunk/Island5.h"
#include "chunk/TileUtil.h"
#include "file/BshFile.h"
#include "file/BshTexture.h"
#include "camera/OrthographicCamera.h"
#include "gl/Texture.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::renderer::IslandModel::IslandModel(Zoom& t_zoom, chunk::Island5* t_parentIsland, std::shared_ptr<file::BshFile> t_bshFile)
    : m_zoom{ t_zoom }
    , m_parentIsland{ t_parentIsland }
    , m_bshFile{ std::move(t_bshFile) }
{
    SG_ASSERT(m_parentIsland, "[IslandModel::IslandModel()] Null pointer.");

    // todo: the Island instance should have an Id
    Log::SG_LOG_DEBUG("[IslandModel::IslandModel()] Create IslandModel.");

    Init();
}

sg::renderer::IslandModel::~IslandModel()
{
    Log::SG_LOG_DEBUG("[IslandModel::~IslandModel()] Destruct IslandModel.");
}

//-------------------------------------------------
// Render
//-------------------------------------------------

void sg::renderer::IslandModel::Render(const camera::OrthographicCamera& t_camera)
{
    OpenGL::EnableAlphaBlending();

    m_shader.Bind();
    glBindVertexArray(m_vao);

    gl::Texture::BindForReading(m_textureArrayId, GL_TEXTURE0, GL_TEXTURE_2D_ARRAY);

    m_shader.SetUniform("viewProjection", t_camera.GetViewProjectionMatrix());
    m_shader.SetUniform("sampler", 0);
    m_shader.SetUniform("maxY", static_cast<float>(m_bshFile->GetMaxY()));

    glDrawArraysInstanced(GL_TRIANGLES, 0, DRAW_COUNT, static_cast<uint32_t>(m_modelMatrices.size()));

    glBindVertexArray(0);
    gl::Shader::Unbind();

    OpenGL::DisableBlending();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::chunk::Island5* sg::renderer::IslandModel::GetParentIsland() const noexcept
{
    return m_parentIsland;
}

//-------------------------------------------------
// Physics
//-------------------------------------------------

sg::physics::Aabb sg::renderer::IslandModel::GetAabb() const noexcept
{
    return m_aabb;
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::renderer::IslandModel::Init()
{
    Log::SG_LOG_DEBUG("[IslandModel::Init()] Start initializing the Island model...");

    CreateGraphicTiles();
    CreateModelMatrices();
    CreateTextureIndex();

    InitShader();

    CreateMesh();

    AddModelMatricesVbo();
    AddTextureIndexVbo();
    AddYVbo();

    CreateTextureArray();

    CreateAabb();

    Log::SG_LOG_DEBUG("[IslandModel::Init()] Island model initializing successfully.");
}

//-------------------------------------------------
// Tile data
//-------------------------------------------------

void sg::renderer::IslandModel::CreateGraphicTiles()
{
    for (auto y{ m_parentIsland->GetIsland5Data().posy }; y < m_parentIsland->GetIsland5Data().posy + m_parentIsland->GetIsland5Data().height; y++)
    {
        for (auto x{ m_parentIsland->GetIsland5Data().posx }; x < m_parentIsland->GetIsland5Data().posx + m_parentIsland->GetIsland5Data().width; x++)
        {
            const auto tile{ m_parentIsland->GetTileFromLayer(x - m_parentIsland->GetIsland5Data().posx, y - m_parentIsland->GetIsland5Data().posy) };
            const auto tileGfxInfo{ m_parentIsland->GetTileGfxInfo(tile) };
            const auto& bshTexture{ m_bshFile->GetBshTexture(tileGfxInfo.gfxIndex) };

            chunk::TileGraphic tileGraphic;
            tileGraphic.tileGfxInfo = tileGfxInfo;
            tileGraphic.mapPosition.x = x;
            tileGraphic.mapPosition.y = y;

            auto screenPosition{ chunk::TileUtil::MapToScreen(x, y, m_zoom.GetXRaster(), m_zoom.GetYRaster()) };

            const auto adjustHeight{ chunk::TileUtil::AdjustHeight(
                m_zoom.GetYRaster(),
                static_cast<int>(tileGraphic.tileGfxInfo.tileHeight),
                m_zoom.GetElevation())
            };

            screenPosition.y += adjustHeight;

            screenPosition.x -= bshTexture.width;
            screenPosition.y -= bshTexture.height;

            tileGraphic.screenPosition = glm::vec2(screenPosition.x, screenPosition.y);
            tileGraphic.size = glm::vec2(bshTexture.width, bshTexture.height);

            m_graphicTiles.push_back(tileGraphic);
        }
    }
}

void sg::renderer::IslandModel::CreateModelMatrices()
{
    for (const auto& tile : m_graphicTiles)
    {
        m_modelMatrices.push_back(tile.GetModelMatrix());
    }
}

void sg::renderer::IslandModel::CreateTextureIndex()
{
    m_textureBuffer.resize(m_graphicTiles.size());
    m_yBuffer.resize(m_graphicTiles.size());

    std::unordered_set<int> islandTexturesToLoad;
    for (const auto& tile : m_graphicTiles)
    {
        islandTexturesToLoad.emplace(tile.tileGfxInfo.gfxIndex);
    }

    Log::SG_LOG_DEBUG("[IslandModel::CreateTextureIndex()] The Island needs {} different textures.", islandTexturesToLoad.size());

    // todo: In this case, the Island should be render (completely or partially) tile by tile.
    SG_ASSERT(islandTexturesToLoad.size() <= MAX_TEXTURES, "[IslandModel::CreateTextureIndex()] Invalid number of textures.");

    auto zOffset{ 0 };
    for (auto toLoad : islandTexturesToLoad)
    {
        m_gfxIndexMap.emplace(toLoad, zOffset);
        zOffset++;
    }

    auto instance{ 0 };
    for (const auto& tile : m_graphicTiles)
    {
        m_textureBuffer[instance] = m_gfxIndexMap.at(tile.tileGfxInfo.gfxIndex);
        m_yBuffer[instance] = tile.size.y;

        instance++;
    }
}

//-------------------------------------------------
// Physics
//-------------------------------------------------

void sg::renderer::IslandModel::CreateAabb()
{
    auto islandScreenPosition{ chunk::TileUtil::MapToScreen(
        m_parentIsland->GetIsland5Data().posx, m_parentIsland->GetIsland5Data().posy,
    m_zoom.GetXRaster(), m_zoom.GetYRaster())
    };

    const auto islandWidth{ m_parentIsland->GetIsland5Data().width };
    const auto islandHeight{ m_parentIsland->GetIsland5Data().height };

    islandScreenPosition.x -= islandWidth / 2 * m_zoom.GetDefaultTileWidth();

    const auto islandScreenSize{ glm::vec2(
        static_cast<float>(islandWidth) * static_cast<float>(m_zoom.GetDefaultTileWidth()),
        static_cast<float>(islandHeight) * static_cast<float>(m_zoom.GetDefaultTileHeight()))
    };

    m_aabb = {
        islandScreenPosition,
        glm::vec2(islandScreenSize.x, islandScreenSize.y)
    };
}

//-------------------------------------------------
// Shader
//-------------------------------------------------

void sg::renderer::IslandModel::InitShader()
{
    m_shader.AddUniform("viewProjection");
    m_shader.AddUniform("sampler");
    m_shader.AddUniform("maxY");
}

//-------------------------------------------------
// Mesh
//-------------------------------------------------

void sg::renderer::IslandModel::CreateMesh()
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

void sg::renderer::IslandModel::AddModelMatricesVbo()
{
    // bind VAO
    glBindVertexArray(m_vao);

    // create VBO
    uint32_t vbo;
    glGenBuffers(1, &vbo);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // stora data
    glBufferData(GL_ARRAY_BUFFER, m_modelMatrices.size() * sizeof(glm::mat4), m_modelMatrices.data(), GL_STATIC_DRAW);

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

void sg::renderer::IslandModel::AddTextureIndexVbo()
{
    // bind VAO
    glBindVertexArray(m_vao);

    // create VBO
    uint32_t vbo;
    glGenBuffers(1, &vbo);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // store data
    glBufferData(GL_ARRAY_BUFFER, m_textureBuffer.size() * sizeof(int), m_textureBuffer.data(), GL_STATIC_DRAW);

    // set layout
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, 4, nullptr);

    glVertexAttribDivisor(5, 1);

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind VAO
    glBindVertexArray(0);
}

void sg::renderer::IslandModel::AddYVbo()
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

void sg::renderer::IslandModel::CreateTextureArray()
{
    const auto maxX{ m_bshFile->GetMaxX() };
    const auto maxY{ m_bshFile->GetMaxY() };

    m_textureArrayId = gl::Texture::GenerateNewTextureId();
    gl::Texture::Bind(m_textureArrayId, GL_TEXTURE_2D_ARRAY);
    glTextureStorage3D(m_textureArrayId, MIP_LEVEL_COUNT, GL_RGBA8, maxX, maxY, static_cast<int32_t>(m_gfxIndexMap.size()));

    std::vector<int> empty(static_cast<size_t>(maxX) * maxY, 0);

    for (const auto& entry : m_gfxIndexMap)
    {
        // key gfx : value pos

        const auto& currentTexture{ m_bshFile->GetBshTexture(entry.first) };

        glTextureSubImage3D(
            m_textureArrayId,
            0,
            0, 0,
            entry.second, // zOffset
            maxX, maxY,
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
