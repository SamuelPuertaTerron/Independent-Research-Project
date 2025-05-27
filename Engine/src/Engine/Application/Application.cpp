#include "Engine.h"
#include "Application.h"

#include "Audio.h"
#include "IApp.h"
#include "IImGuiWindow.h"
#include "Window.h"
#include "../Render/Render.h"

namespace Engine
{
	void Application::RunApplication(const std::shared_ptr<IApp>& app)
	{
		SE_ASSERT(app != nullptr, "The IApp is  not correctly created");
		m_App = app;

		/* Initialize timing */
		m_LastFrameTime = std::chrono::high_resolution_clock::now();
		m_DeltaTime = 0.0f;
		m_FPS = 0.0f;
		m_FrameCount = 0;
		m_FPSTimer = 0.0f;

		/* Don't care about the return type as it call an error inside the function */
		Init();

		while (m_bIsRunning)
		{
			// Calculate delta time
			auto now = std::chrono::high_resolution_clock::now();
			m_DeltaTime = std::chrono::duration<float>(now - m_LastFrameTime).count();
			m_LastFrameTime = now;

			// Update FPS counter
			UpdateFPSCounter();

			m_Window->PollEvents();
			Tick();
		}

		Destroy();
	}

	void Application::Quit()
	{
		m_bIsRunning = false;
	}

	bool Application::Init()
	{
		m_App->OnAppLoadSettings();

		Render::g_GraphicsAPI = m_App->GetSettings().GraphicsAPI;

		m_App->PreInit();

		Logger::Log("Created App", ELogLevel::Info);
		m_Window = std::make_shared<Window>();
		SE_ASSERT(m_Window->Init(m_App->GetSettings()), "Failed to initialize the Window on Window::Init()");

		SE_ASSERT(Renderer::Init(), "Failed to initialize the renderer");

		m_ImGuiWindow = IImGuiWindow::Create();
		SE_ASSERT(m_ImGuiWindow->Init(), "Failed to initialize the ImGui Window");

		Audio::GetInstance().Init();

		m_App->Init();

		m_bIsRunning = true;
		return true;
	}

	void Application::Tick()
	{
		m_Window->PollEvents();

		Renderer::BeginRender();
		m_ImGuiWindow->BeginRender();

		m_App->Tick(m_DeltaTime);

		m_ImGuiWindow->EndRender();
		Renderer::EndRender();

	}

	void Application::Destroy()
	{
		m_App->Destroy();

		Audio::GetInstance().Stop();
		Audio::GetInstance().Destroy();

		m_ImGuiWindow->Destroy();

		Renderer::Destroy();

		m_Window->Destroy();

		m_App->LateDestroy();

		Logger::Log("Destroyed App", ELogLevel::Info);
		/* The last thing to be destroyed */
		Release();
	}
	void Application::UpdateFPSCounter()
	{
		m_FPSTimer += m_DeltaTime;
		m_FrameCount++;

		if (m_FPSTimer >= 1.0f) // Update FPS every second
		{
			m_FPS = static_cast<float>(m_FrameCount) / m_FPSTimer;
			m_FrameCount = 0;
			m_FPSTimer = 0.0f;
		}
	}
	std::shared_ptr<IApp> Application::GetApp() const
	{
		return m_App;
	}
	std::shared_ptr<Window> Application::GetWindow() const
	{
		return m_Window;
	}
}//namespace Engine