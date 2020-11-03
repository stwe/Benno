#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "OpenGL.h"
#include "SgAssert.h"
#include "SgException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::gl::Shader::Shader(const std::string& t_vertexShaderFilePath, const std::string& t_fragmentShaderFilePath)
{
    m_programId = glCreateProgram();
    SG_ASSERT(m_programId, "[Shader::Shader()] Error while creating a new shader program.");

    Log::SG_LOG_DEBUG("[Shader::Shader()] A new shader program was created. Id: {}", m_programId);

    AddVertexShader(ReadShaderFile(t_vertexShaderFilePath));
    AddFragmentShader(ReadShaderFile(t_fragmentShaderFilePath));

    LinkAndValidateProgram();

    Log::SG_LOG_DEBUG("[Shader::Shader()] All shader was added successfully.");
}

sg::gl::Shader::~Shader()
{
    Log::SG_LOG_DEBUG("[Shader::~Shader()] Destruct shader program.");
    CleanUp();
}

//-------------------------------------------------
// Getter / read-only
//-------------------------------------------------

uint32_t sg::gl::Shader::GetProgramId() const noexcept
{
    return m_programId;
}

//-------------------------------------------------
// Add uniforms
//-------------------------------------------------

void sg::gl::Shader::AddUniform(const std::string& t_uniformName)
{
    auto uniformId{ glGetUniformLocation(m_programId, t_uniformName.c_str()) };
    SG_ASSERT(uniformId != -1, "[Shader::AddUniform()] Invalid uniform variable.");

    m_uniforms.emplace(t_uniformName, uniformId);
}

//-------------------------------------------------
// Bind / Unbind
//-------------------------------------------------

void sg::gl::Shader::Bind() const
{
    SG_ASSERT(m_programId, "[Shader::Bind()] Invalid shader program Id.");
    glUseProgram(m_programId);
}

void sg::gl::Shader::Unbind()
{
    glUseProgram(0);
}

//-------------------------------------------------
// Set uniforms
//-------------------------------------------------

void sg::gl::Shader::SetUniform(const std::string& t_uniformName, const int32_t t_value)
{
    glUniform1i(m_uniforms.at(t_uniformName), t_value);
}

void sg::gl::Shader::SetUniform(const std::string& t_uniformName, const float t_value)
{
    glUniform1f(m_uniforms.at(t_uniformName), t_value);
}

void sg::gl::Shader::SetUniform(const std::string& t_uniformName, const bool t_value)
{
    // if value == true load 1 else 0 as float
    glUniform1f(m_uniforms.at(t_uniformName), t_value ? 1.0f : 0.0f);
}

void sg::gl::Shader::SetUniform(const std::string& t_uniformName, const glm::vec2& t_value)
{
    glUniform2f(m_uniforms.at(t_uniformName), t_value.x, t_value.y);
}

void sg::gl::Shader::SetUniform(const std::string& t_uniformName, const glm::vec3& t_value)
{
    glUniform3f(m_uniforms.at(t_uniformName), t_value.x, t_value.y, t_value.z);
}

void sg::gl::Shader::SetUniform(const std::string& t_uniformName, const glm::vec4& t_value)
{
    glUniform4f(m_uniforms.at(t_uniformName), t_value.x, t_value.y, t_value.z, t_value.w);
}

void sg::gl::Shader::SetUniform(const std::string& t_uniformName, const glm::mat4& t_value)
{
    glUniformMatrix4fv(m_uniforms.at(t_uniformName), 1, GL_FALSE, value_ptr(t_value));
}

//-------------------------------------------------
// Add shader types
//-------------------------------------------------

void sg::gl::Shader::AddVertexShader(const std::string& t_shaderCode)
{
    m_vertexShaderId = AddShader(t_shaderCode, GL_VERTEX_SHADER);
    Log::SG_LOG_DEBUG("[Shader::AddVertexShader()] A new vertex shader was added. Id: {}", m_vertexShaderId);
}

void sg::gl::Shader::AddFragmentShader(const std::string& t_shaderCode)
{
    m_fragmentShaderId = AddShader(t_shaderCode, GL_FRAGMENT_SHADER);
    Log::SG_LOG_DEBUG("[Shader::AddFragmentShader()] A new fragment shader was added. Id: {}", m_fragmentShaderId);
}

//-------------------------------------------------
// Link
//-------------------------------------------------

void sg::gl::Shader::LinkAndValidateProgram() const
{
    SG_ASSERT(m_programId, "[Shader::LinkAndValidateProgram()] Invalid shader program Id.");

    // link our program
    glLinkProgram(m_programId);

    // error handling
    auto isLinked{ GL_FALSE };
    glGetProgramiv(m_programId, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        auto maxLength{ 0 };
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(m_programId, maxLength, &maxLength, &infoLog[0]);

        CleanUp();

        std::string log;
        for (const auto& value : infoLog)
        {
            log.push_back(value);
        }

        throw SG_EXCEPTION("[Shader::LinkAndValidate()] Error while linking shader program. Log: " + log);
    }

    // cleanup: always detach shaders after a successful link
    if (m_vertexShaderId != 0)
    {
        glDetachShader(m_programId, m_vertexShaderId);
    }

    if (m_fragmentShaderId != 0)
    {
        glDetachShader(m_programId, m_fragmentShaderId);
    }

    // validate our program
    glValidateProgram(m_programId);

    auto isValidated{ GL_FALSE };
    glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &isValidated);
    if (isValidated == GL_FALSE)
    {
        auto maxLength{ 0 };
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(m_programId, maxLength, &maxLength, &infoLog[0]);

        CleanUp();

        std::string log;
        for (const auto& value : infoLog)
        {
            log.push_back(value);
        }

        throw SG_EXCEPTION("[Shader::LinkAndValidate()] Shader program validation error. Log: " + log);
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

std::string sg::gl::Shader::ReadShaderFile(const std::string& t_fileName)
{
    std::string content;
    std::ifstream shaderFile;

    // Ensure ifstream objects can throw exceptions.
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Open file.
        shaderFile.open(t_fileName);
        std::stringstream shaderStream;

        // Read file buffer content into stream.
        shaderStream << shaderFile.rdbuf();

        // Close file.
        shaderFile.close();

        // Convert stream into string.
        content = shaderStream.str();
    }
    catch (const std::ifstream::failure&)
    {
        throw SG_EXCEPTION("[Shader::ReadShaderFile()] Exception caught while loading of file " + t_fileName + ".");
    }

    return content;
}

uint32_t sg::gl::Shader::GenerateShader(const int32_t t_shaderType)
{
    const auto shaderId{ glCreateShader(t_shaderType) };
    SG_ASSERT(shaderId, "[Shader::GenerateShader()] Error while creating shader object.");

    return shaderId;
}

void sg::gl::Shader::CompileShader(const uint32_t t_shaderId, const std::string& t_shaderCode)
{
    const auto* shaderSrc{ t_shaderCode.c_str() };
    glShaderSource(t_shaderId, 1, &shaderSrc, nullptr);
    glCompileShader(t_shaderId);
}

void sg::gl::Shader::CheckCompileStatus(const uint32_t t_shaderId)
{
    auto isCompiled{ GL_FALSE };
    glGetShaderiv(t_shaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        auto maxLength{ 0 };
        glGetShaderiv(t_shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(t_shaderId, maxLength, &maxLength, &errorLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(t_shaderId);

        std::string log;
        for (const auto& value : errorLog)
        {
            log.push_back(value);
        }

        throw SG_EXCEPTION("[Shader::CheckCompileStatus()] Error while compiling shader program. Log: " + log);
    }
}

uint32_t sg::gl::Shader::AddShader(const std::string& t_shaderCode, const int32_t t_shaderType) const
{
    const auto shaderId{ GenerateShader(t_shaderType) };
    CompileShader(shaderId, t_shaderCode);
    CheckCompileStatus(shaderId);
    glAttachShader(m_programId, shaderId);

    return shaderId;
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::gl::Shader::CleanUp() const
{
    Log::SG_LOG_DEBUG("[Shader::CleanUp()] Start the OpenGL clean up process for shader program. Id: {}", m_programId);

    Unbind();

    if (m_vertexShaderId)
    {
        glDeleteShader(m_vertexShaderId);
        Log::SG_LOG_DEBUG("[Shader::CleanUp()] Vertex shader was deleted. Id: {}", m_vertexShaderId);
    }

    if (m_fragmentShaderId)
    {
        glDeleteShader(m_fragmentShaderId);
        Log::SG_LOG_DEBUG("[Shader::CleanUp()] Fragment shader was deleted. Id: {}", m_fragmentShaderId);
    }

    if (m_programId)
    {
        glDeleteProgram(m_programId);
        Log::SG_LOG_DEBUG("[Shader::CleanUp()] Shader program was deleted. Id: {}", m_programId);
    }
}
