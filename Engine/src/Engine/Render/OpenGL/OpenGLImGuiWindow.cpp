#include "Engine.h"
#include "OpenGLImGuiWindow.h"

#include <Windows.h>

#include "../../ThirdParty/imgui/imgui/imgui.h"
#include "../../ThirdParty/imgui/imgui/backends/imgui_impl_opengl3.h"
#include "../../ThirdParty/imgui/imgui/backends/imgui_impl_win32.h"
#include "Engine/Application/INativeWindow.h"
#include "Engine/Application/Window.h"

#include "Engine/Render/OpenGL/OpenGLRenderContext.h"

namespace Engine::Render::OpenGL
{
	bool OpenGLImGuiWindow::Init()
	{
        const HWND windowHandle = (HWND)Application::GetInstance().GetWindow()->GetNativeWindow()->GetWindowHandle();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_InitForOpenGL(windowHandle);
        ImGui_ImplOpenGL3_Init();

        Logger::Log("Created OpenGL ImGui Backend", ELogLevel::Info);

        return true;
    }

	void OpenGLImGuiWindow::Destroy()
	{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        Logger::Log("Destroyed OpenGL ImGui Backend", ELogLevel::Info);
	}

	void OpenGLImGuiWindow::BeginRender()
	{
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
	}

	void OpenGLImGuiWindow::EndRender()
	{
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}//namespace Engine::Render::OpenGL