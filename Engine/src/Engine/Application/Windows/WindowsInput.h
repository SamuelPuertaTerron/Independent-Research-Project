/* Windows Input */
#pragma once

#include <Windows.h>

namespace Engine
{
	enum class EKeyCode : uint8_t;
}//namespace Engine

namespace Engine::Win32
{
	class WindowsInput
	{
	public:
		static bool InputWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static EKeyCode ConvertNativeKeyCodes(int key);
	};

	class WindowsKeyboardInput
	{
	public:
		static bool KeyboardWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	class WindowsMouseInput
	{
	public:
		static bool MouseWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

}//namespace Engine::Win32
