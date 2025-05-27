#include "Engine.h"
#include "Window.h"

#include "INativeWindow.h"
#include "Engine/Render/Render.h"

namespace Engine
{
	bool Window::Init(const ApplicationSettings& settings)
	{
		m_WindowSize = settings.Size;

		m_NativeWindow = INativeWindow::Create();
		SE_ASSERT(m_NativeWindow != nullptr, "Native Window is null. Currently only supports Windows");
		if (!m_NativeWindow->Init())
		{
			return false;
		}

		Logger::Log("Created Native Window", ELogLevel::Info);
		return true;
	}

	bool Window::LateInit() const
	{
		return m_NativeWindow->LateInit();
	}

	void Window::Destroy() const
	{
		m_NativeWindow->Destroy();
		Logger::Log("Destroyed Native Window", ELogLevel::Info);
	}

	void Window::EarlyDestroy() const
	{
		m_NativeWindow->EarlyDestroy();
	}

	void Window::PollEvents() const
	{
		m_NativeWindow->PollEvents();
	}

	void Window::SwapBuffers() const
	{
		m_NativeWindow->SwapBuffers();
	}

	Vector2Int Window::GetWindowSize() const
	{
		return m_WindowSize;
	}

	void Window::SetWindowSize(Vector2Int size)
	{
		m_WindowSize = size;
		Renderer::SetViewportSize(m_WindowSize);
	}

	std::shared_ptr<INativeWindow> Window::GetNativeWindow() const
	{
		return m_NativeWindow;
	}

	const char* GetProcAddress(const char* name)
	{
		return static_cast<const char*>(Application::GetInstance().GetWindow()->GetNativeWindow()->GetProcAddress(name));
	}

}//namespace Engine