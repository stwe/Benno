#pragma once

#include <memory>
#include <unordered_map>
#include <string>

namespace sg::gl
{
    class Shader;

    class ShaderManager
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ShaderManager();

        ShaderManager(const ShaderManager& t_other) = delete;
        ShaderManager(ShaderManager&& t_other) noexcept = delete;
        ShaderManager& operator=(const ShaderManager& t_other) = delete;
        ShaderManager& operator=(ShaderManager&& t_other) noexcept = delete;

        ~ShaderManager();

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] Shader& GetShader(const std::string& t_id);
        [[nodiscard]] const Shader& GetShader(const std::string& t_id) const;

        //-------------------------------------------------
        // Add
        //-------------------------------------------------

        void AddShader(const std::string& t_id);

    protected:

    private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaderMap;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        [[nodiscard]] bool IsShaderNotLoaded(const std::string& t_id) const;
    };
}
