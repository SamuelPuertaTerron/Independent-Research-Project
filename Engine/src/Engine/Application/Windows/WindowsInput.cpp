#include "Engine.h"
#include "WindowsInput.h"

#include "Engine/Application/Input/InputManager.h"
#include "Engine/Application/Input/KeyCodes.h"

namespace Engine::Win32
{
	bool WindowsInput::InputWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (WindowsKeyboardInput::KeyboardWindowProc(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}

		if (WindowsMouseInput::MouseWindowProc(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}

		return false;
	}

	EKeyCode WindowsInput::ConvertNativeKeyCodes(int key)
	{
		const std::unordered_map<int, EKeyCode> keyMap =
		{
			{ 'A', EKeyCode::A }, { 'B', EKeyCode::B }, { 'C', EKeyCode::C }, { 'D', EKeyCode::D },
			{ 'E', EKeyCode::E }, { 'F', EKeyCode::F }, { 'G', EKeyCode::G }, { 'H', EKeyCode::H },
			{ 'I', EKeyCode::I }, { 'J', EKeyCode::J }, { 'K', EKeyCode::K }, { 'L', EKeyCode::L },
			{ 'M', EKeyCode::M }, { 'N', EKeyCode::N }, { 'O', EKeyCode::O }, { 'P', EKeyCode::P },
			{ 'Q', EKeyCode::Q }, { 'R', EKeyCode::R }, { 'S', EKeyCode::S }, { 'T', EKeyCode::T },
			{ 'U', EKeyCode::U }, { 'V', EKeyCode::V }, { 'W', EKeyCode::W }, { 'X', EKeyCode::X },
			{ 'Y', EKeyCode::Y }, { 'Z', EKeyCode::Z },

			{ VK_ESCAPE, EKeyCode::Escape }, { VK_RETURN, EKeyCode::Enter },
			{ VK_SPACE, EKeyCode::Space },   { VK_BACK, EKeyCode::Backspace },
			{ VK_TAB, EKeyCode::Tab },		   { VK_SHIFT, EKeyCode::Shift },
			{ VK_CONTROL, EKeyCode::Ctrl },	{ VK_MENU, EKeyCode::Alt },

			{ VK_LEFT, EKeyCode::Left },		{ VK_RIGHT, EKeyCode::Right },
			{ VK_UP, EKeyCode::Up },			{ VK_DOWN, EKeyCode::Down },

			{ VK_F1, EKeyCode::F1 },			{ VK_F2, EKeyCode::F2 },		{ VK_F3, EKeyCode::F3 },
			{ VK_F4, EKeyCode::F4 },			{ VK_F5, EKeyCode::F5 },		{ VK_F6, EKeyCode::F6 },
			{ VK_F7, EKeyCode::F7 },			{ VK_F8, EKeyCode::F8 },		{ VK_F9, EKeyCode::F9 },
			{ VK_F10, EKeyCode::F10 },		{ VK_F11, EKeyCode::F11 },	{ VK_F12, EKeyCode::F12 }
		};

		auto it = keyMap.find(key);
		return (it != keyMap.end()) ? it->second : EKeyCode::Unknown;
	}

	bool WindowsKeyboardInput::KeyboardWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		bool result = false;

		switch (uMsg)
		{
		case WM_CHAR:
			// Handle character input if needed
			result = true;
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (InputManager::IsKeyCallbackValid())
			{
				result = true;
				EKeyCode key = WindowsInput::ConvertNativeKeyCodes(static_cast<int>(wParam));
				EInputAction action = (lParam & (1 << 30)) ? EInputAction::Down : EInputAction::Press;
				InputManager::CallKeyCallback(key, action);
			}
			else
			{
				EKeyCode key = WindowsInput::ConvertNativeKeyCodes(static_cast<int>(wParam));
				EInputAction action = (lParam & (1 << 30)) ? EInputAction::Down : EInputAction::Press;
				InputManager::KeyDown(key, action);
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (InputManager::IsKeyCallbackValid())
			{
				result = true;
				EKeyCode key = WindowsInput::ConvertNativeKeyCodes(static_cast<int>(wParam));
				InputManager::CallKeyCallback(key, EInputAction::Release);
			}
			else
			{
				EKeyCode key = WindowsInput::ConvertNativeKeyCodes(static_cast<int>(wParam));
				InputManager::KeyUp(key, EInputAction::Release);
			}
			break;
		}

		return result;
	}

	bool WindowsMouseInput::MouseWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			InputManager::MouseDown(EMouseButton::Left);
			break;
		case WM_LBUTTONUP:
			InputManager::MouseUp(EMouseButton::Left);
			break;
		case WM_RBUTTONDOWN:
			InputManager::MouseDown(EMouseButton::Right);
			break;
		case WM_RBUTTONUP:
			InputManager::MouseUp(EMouseButton::Right);

			break;
		case WM_MBUTTONDOWN:
			InputManager::MouseDown(EMouseButton::Middle);
			break;
		case WM_MBUTTONUP:
			InputManager::MouseUp(EMouseButton::Middle);
			break;
		}

		return false;
	}
}//namespace Engine