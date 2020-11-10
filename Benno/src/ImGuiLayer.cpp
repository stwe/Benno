#include "ImGuiLayer.h"
#include "Game.h"
#include "Window.h"
#include "vendor/imgui/imgui_impl_sdl.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ImGuiLayer::ImGuiLayer(Game* t_parentGame)
    : Layer(t_parentGame)
{
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::ImGuiLayer::OnAttach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io{ ImGui::GetIO() };
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(GetParentGame()->GetWindow().GetSdlWindow(), GetParentGame()->GetWindow().GetContext());
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void sg::ImGuiLayer::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void sg::ImGuiLayer::OnEvent()
{
}

//-------------------------------------------------
// Frame
//-------------------------------------------------

void sg::ImGuiLayer::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(GetParentGame()->GetWindow().GetSdlWindow());
    ImGui::NewFrame();
}

void sg::ImGuiLayer::End()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
