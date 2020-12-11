#include "ShaderManager.h"
#include "Log.h"
#include "SgException.h"
#include "Shader.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::gl::ShaderManager::ShaderManager()
{
    Log::SG_LOG_DEBUG("[ShaderManager::ShaderManager()] Create ShaderManager.");
}

sg::gl::ShaderManager::~ShaderManager()
{
    Log::SG_LOG_DEBUG("[ShaderManager::~ShaderManager()] Destruct ShaderManager.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::gl::Shader& sg::gl::ShaderManager::GetShader(const std::string& t_id)
{
    if (IsShaderNotLoaded(t_id))
    {
        throw SG_EXCEPTION("[ShaderManager::GetShader()] Shader " + t_id + " does not exist.");
    }

    return *m_shaderMap.at(t_id);
}

const sg::gl::Shader& sg::gl::ShaderManager::GetShader(const std::string& t_id) const
{
    if (IsShaderNotLoaded(t_id))
    {
        throw SG_EXCEPTION("[ShaderManager::GetShader()] Shader " + t_id + " does not exist.");
    }

    return *m_shaderMap.at(t_id);
}

//-------------------------------------------------
// Add
//-------------------------------------------------

void sg::gl::ShaderManager::AddShader(const std::string& t_id)
{
    if (IsShaderNotLoaded(t_id))
    {
        auto vertexShader{ "res/shader/" + t_id + "/Vertex.vert" };
        auto fragmentShader{ "res/shader/" + t_id + "/Fragment.frag" };

        auto shader{ std::make_unique<Shader>(vertexShader, fragmentShader) };
        m_shaderMap.emplace(t_id, std::move(shader));
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

bool sg::gl::ShaderManager::IsShaderNotLoaded(const std::string& t_id) const
{
    return m_shaderMap.count(t_id) == 0 ? true : false;
}
