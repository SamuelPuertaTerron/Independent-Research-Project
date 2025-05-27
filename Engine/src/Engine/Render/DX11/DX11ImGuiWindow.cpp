#include "Engine.h"
#include "DX11ImGuiWindow.h"

#include "DX11RenderContext.h"
#include "../../ThirdParty/imgui/imgui/imgui.h"
#include "../../ThirdParty/imgui/imgui/backends/imgui_impl_dx11.h"
#include "../../ThirdParty/imgui/imgui/backends/imgui_impl_win32.h"
#include "Engine/Application/INativeWindow.h"
#include "Engine/Application/Window.h"
#include "Engine/Render/Render.h"

namespace Engine::Render::DX11
{
	bool DX11ImGuiWindow::Init()
	{
		const HWND windowHandle = (HWND)Application::GetInstance().GetWindow()->GetNativeWindow()->GetWindowHandle();
		const DX11RenderContext* renderer = dynamic_cast<DX11RenderContext*>(Renderer::GetRenderContext().get());

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(windowHandle);
		ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetImmediateContext());

		Logger::Log("Created DX11 ImGui Backend", ELogLevel::Info);

		return true;
	}

	void DX11ImGuiWindow::Destroy()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		Logger::Log("Destroyed DX11 ImGui Backend", ELogLevel::Info);
	}

	void DX11ImGuiWindow::BeginRender()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void DX11ImGuiWindow::EndRender()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}//namespace Engine::Render::DX11