#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace sg::gl
{
    class Shader
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Shader() = delete;

        Shader(const std::string& t_vertexShaderFilePath, const std::string& t_fragmentShaderFilePath);

        Shader(const Shader& t_other) = delete;
        Shader(Shader&& t_other) noexcept = delete;
        Shader& operator=(const Shader& t_other) = delete;
        Shader& operator=(Shader&& t_other) noexcept = delete;

        ~Shader();

        //-------------------------------------------------
        // Getter / read-only
        //-------------------------------------------------

        [[nodiscard]] uint32_t GetProgramId() const noexcept;

        //-------------------------------------------------
        // Add uniforms
        //-------------------------------------------------

        void AddUniform(const std::string& t_uniformName);

        //-------------------------------------------------
        // Bind / Unbind
        //-------------------------------------------------

        void Bind() const;
        static void Unbind();

        //-------------------------------------------------
        // Set uniforms
        //-------------------------------------------------

        void SetUniform(const std::string& t_uniformName, int32_t t_value);
        void SetUniform(const std::string& t_uniformName, float t_value);
        void SetUniform(const std::string& t_uniformName, bool t_value);
        void SetUniform(const std::string& t_uniformName, const glm::vec2& t_value);
        void SetUniform(const std::string& t_uniformName, const glm::vec3& t_value);
        void SetUniform(const std::string& t_uniformName, const glm::vec4& t_value);
        void SetUniform(const std::string& t_uniformName, const glm::mat4& t_value);

    protected:

    private:
        uint32_t m_programId{ 0 };

        uint32_t m_vertexShaderId{ 0 };
        uint32_t m_fragmentShaderId{ 0 };

        std::unordered_map<std::string, int32_t> m_uniforms;

        //-------------------------------------------------
        // Add shader types
        //-------------------------------------------------

        void AddVertexShader(const std::string& t_shaderCode);
        void AddFragmentShader(const std::string& t_shaderCode);

        //-------------------------------------------------
        // Link
        //-------------------------------------------------

        void LinkAndValidateProgram() const;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static std::string ReadShaderFile(const std::string& t_fileName);

        static uint32_t GenerateShader(int32_t t_shaderType);
        static void CompileShader(uint32_t t_shaderId, const std::string& t_shaderCode);
        static void CheckCompileStatus(uint32_t t_shaderId);

        [[nodiscard]] uint32_t AddShader(const std::string& t_shaderCode, int32_t t_shaderType) const;

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;
    };
}
