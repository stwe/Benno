#pragma once

#include <memory>
#include <unordered_map>
#include <glm/vec2.hpp>
#include "Layer.h"
#include "renderer/Zoom.h"

namespace sg::renderer
{
    class MeshRenderer;
}

namespace sg::camera
{
    class OrthographicCamera;
}

namespace sg::data
{
    class HousesJsonFile;
}

namespace sg::file
{
    class PaletteFile;
    class BshFile;
    class GamFile;
}

namespace sg
{
    class Game;

    class GameLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        GameLayer() = delete;

        GameLayer(Game* t_parentGame, const std::string& t_name);

        GameLayer(const GameLayer& t_other) = delete;
        GameLayer(GameLayer&& t_other) noexcept = delete;
        GameLayer& operator=(const GameLayer& t_other) = delete;
        GameLayer& operator=(GameLayer&& t_other) noexcept = delete;

        ~GameLayer() = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] const renderer::Zoom& GetCurrentZoom() const noexcept;
        [[nodiscard]] std::shared_ptr<renderer::MeshRenderer> GetMeshRenderer() const noexcept;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCurrentZoom(renderer::Zoom::ZoomId t_zoomId);

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void OnCreate() override;
        void OnDestruct() override;

        void OnUpdate() override;
        void OnRender() override;
        void OnGuiRender() override;
        void OnSdlEvent(const SDL_Event& t_event) override;
        void OnInput() override;

    protected:

    private:
        renderer::Zoom m_currentZoom{};

        std::unique_ptr<camera::OrthographicCamera> m_camera;
        std::shared_ptr<data::HousesJsonFile> m_housesJsonFile;
        std::unique_ptr<file::PaletteFile> m_paletteFile;
        std::unordered_map<renderer::Zoom::ZoomId, std::shared_ptr<file::BshFile>> m_stadtfldFiles;
        std::unordered_map<renderer::Zoom::ZoomId, std::unique_ptr<file::GamFile>> m_gamFiles;

        std::shared_ptr<renderer::MeshRenderer> m_meshRenderer;
        glm::vec2 m_mapPosition{ 0.0f };

        int m_info{ 0 };
        bool m_renderIslandAabbs{ false };
        bool m_onIsland{ false };
    };
}
