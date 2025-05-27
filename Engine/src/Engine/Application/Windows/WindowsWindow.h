/* A windows window */

#pragma once
#include "../INativeWindow.h"

#include <Windows.h>

namespace Engine::Win32
{
	class WindowsWindow : public INativeWindow
	{
	public:
		WindowsWindow() = default;
		virtual ~WindowsWindow() override = default;

		[[nodiscard]] virtual bool Init() override;
		[[nodiscard]] virtual bool LateInit() override;
		virtual void Destroy() override;
		virtual void EarlyDestroy() override;

		virtual void PollEvents() override;
		virtual void SwapBuffers() override;

		virtual void* GetProcAddress(const char* name) override;

		virtual void* GetWindowHandle() override;
	private:
		LPCWSTR CLASS_NAME = L"Win32WindowClass";

		HWND m_WindowHandle;
		WINDOWPLACEMENT m_WindowPlacement;
		HDC m_DeviceContext;
		HINSTANCE m_Instance;
		HGLRC m_OpenGLContext;
	};
}//namespace Engine::Win32