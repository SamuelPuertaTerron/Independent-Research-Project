#include "Engine.h"
#include "WindowsWindow.h"

#include "../IApp.h"
#include "../Window.h"

#include "WindowsInput.h"
#include "../../../../../ThirdParty/ImGui/imgui/imgui.h"
#include "../../../../../ThirdParty/ImGui/imgui/backends/imgui_impl_dx11.h"
#include "../../../../../ThirdParty/ImGui/imgui/backends/imgui_impl_win32.h"
#include "Engine/Render/Render.h"
#include "Engine/Render/DX11/DX11RenderContext.h"
#include "Engine/Render/OpenGL/OpenGLCreateContext.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Engine::Win32
{
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		switch (uMsg)
		{
		case WM_NCCALCSIZE:
		{
			// Remove the standard window frame
			if (wParam == TRUE)
			{
				NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)lParam;
				params->rgrc[0].top += 30;
				return 0;
			}
			break;
		}
		case WM_SIZE:
			{
				int width = LOWORD(lParam);
				int height = HIWORD(lParam);
				Application::GetInstance().GetWindow()->SetWindowSize({width, height});
			}
			break;
		case WM_DESTROY:
			Application::GetInstance().Quit();
			break;
		case WM_QUIT:
			PostQuitMessage(0);
			break;
		}

		if (WindowsInput::InputWindowProc(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	bool WindowsWindow::Init()
	{
		m_Instance = GetModuleHandle(nullptr);

		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = m_Instance;
		wc.lpszClassName = CLASS_NAME;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

		if (!RegisterClassEx(&wc))
		{
			MessageBox(nullptr, L"RegisterClassEx failed", L"Error", MB_ICONERROR);
			return false;
		}

		const ApplicationSettings windowSettings = Application::GetInstance().GetApp()->GetSettings();

		RECT rect = { 0, 0, (LONG)windowSettings.Size.Width, (LONG)windowSettings.Size.Height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		std::wstring wtitle(windowSettings.Title.begin(), windowSettings.Title.end());
		m_WindowHandle = CreateWindowEx(
			0,
			CLASS_NAME,
			wtitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr, nullptr, m_Instance, nullptr
		);

		if (!m_WindowHandle) {
			MessageBox(nullptr, L"CreateWindowEx returned NULL", L"Error", MB_ICONERROR);
			return false;
		}

		m_DeviceContext = GetDC(m_WindowHandle);
		ShowWindow(m_WindowHandle, SW_SHOW);
		UpdateWindow(m_WindowHandle);

		if (Render::g_GraphicsAPI == Render::EGraphicsAPI::OpenGL)
		{
			m_OpenGLContext = nullptr;
			OpenGL::OpenGLCreateContext::CreateContext(m_DeviceContext, m_OpenGLContext);
		}

		return true;
	}

	bool WindowsWindow::LateInit()
	{
		if (Render::g_GraphicsAPI == Render::EGraphicsAPI::OpenGL)
		{
			
		}
		else if (Render::g_GraphicsAPI == Render::EGraphicsAPI::DX11)
		{

		}

		return true;
	}

	void WindowsWindow::Destroy()
	{
		DestroyWindow(m_WindowHandle);

		UnregisterClass(CLASS_NAME, GetModuleHandle(nullptr));
	}

	void WindowsWindow::EarlyDestroy()
	{

	}

	void WindowsWindow::PollEvents()
	{
		MSG msg = {};
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowsWindow::SwapBuffers()
	{
		/* This is only for OpenGL */
		::SwapBuffers(m_DeviceContext);
	}

	void* WindowsWindow::GetProcAddress(const char* name)
	{
		void* proc = (void*)wglGetProcAddress(name);
		if (!proc)
		{
			//If fails we attempt to load the opengl32.dll
			HMODULE module = LoadLibraryA("opengl32.dll");
			if (module)
			{
				proc = (void*)::GetProcAddress(module, name);
			}
		}
		return proc;

	}
	void* WindowsWindow::GetWindowHandle()
	{
		if (m_WindowHandle)
		{
			return m_WindowHandle;
		}

		return nullptr;
	}
}//namespace Engine::Win32