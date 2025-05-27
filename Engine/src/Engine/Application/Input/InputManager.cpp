#include "Engine.h"
#include "InputManager.h"

#include "KeyCodes.h"

namespace Engine
{
	void InputManager::Init()
	{
		for (int i = 0; i < 256; i++)
		{
			m_Keys[i].Code = EKeyCode::Unknown;
			m_Keys[i].Action = EInputAction::None;
		}

		for (int i = 0; i < 5; i++)
		{
			m_MouseButtons[EMouseButton::Unknown] = false;
		}
	}

	bool InputManager::IsKeyDown(EKeyCode code)
	{
		return m_Keys[static_cast<uint8_t>(code)].Action == EInputAction::Down;
	}

	bool InputManager::IsKeyPressed(EKeyCode code)
	{
		return m_Keys[static_cast<uint8_t>(code)].Action == EInputAction::Press;
	}

	bool InputManager::IsKeyReleased(EKeyCode code)
	{
		return m_Keys[static_cast<uint8_t>(code)].Action == EInputAction::Release;
	}

	void InputManager::KeyDown(EKeyCode code, EInputAction action)
	{
		m_Keys[static_cast<uint8_t>(code)].Action = action;
	}

	void InputManager::KeyUp(EKeyCode code, EInputAction action)
	{
		m_Keys[static_cast<uint8_t>(code)].Action = action;
	}

	bool InputManager::IsMouseDown(EMouseButton button)
	{
		return m_MouseButtons[button];
	}

	void InputManager::MouseDown(EMouseButton button)
	{
		m_MouseButtons[button] = true;
	}

	void InputManager::MouseUp(EMouseButton button)
	{
		m_MouseButtons[button] = false;
	}


	void InputManager::CallKeyCallback(EKeyCode code, EInputAction action)
	{
	}

	bool InputManager::IsKeyCallbackValid()
	{
		return false;
	}

	std::string InputManager::GetKeyCodeAsString(const EKeyCode& code)
	{
		switch (code)
		{
			case EKeyCode::Unknown: return "Unknown";

				// Alphabet
			case EKeyCode::A: return "A";
			case EKeyCode::B: return "B";
			case EKeyCode::C: return "C";
			case EKeyCode::D: return "D";
			case EKeyCode::E: return "E";
			case EKeyCode::F: return "F";
			case EKeyCode::G: return "G";
			case EKeyCode::H: return "H";
			case EKeyCode::I: return "I";
			case EKeyCode::J: return "J";
			case EKeyCode::K: return "K";
			case EKeyCode::L: return "L";
			case EKeyCode::M: return "M";
			case EKeyCode::N: return "N";
			case EKeyCode::O: return "O";
			case EKeyCode::P: return "P";
			case EKeyCode::Q: return "Q";
			case EKeyCode::R: return "R";
			case EKeyCode::S: return "S";
			case EKeyCode::T: return "T";
			case EKeyCode::U: return "U";
			case EKeyCode::V: return "V";
			case EKeyCode::W: return "W";
			case EKeyCode::X: return "X";
			case EKeyCode::Y: return "Y";
			case EKeyCode::Z: return "Z";

				// Numbers
			case EKeyCode::Num0: return "0";
			case EKeyCode::Num1: return "1";
			case EKeyCode::Num2: return "2";
			case EKeyCode::Num3: return "3";
			case EKeyCode::Num4: return "4";
			case EKeyCode::Num5: return "5";
			case EKeyCode::Num6: return "6";
			case EKeyCode::Num7: return "7";
			case EKeyCode::Num8: return "8";
			case EKeyCode::Num9: return "9";

				// Special keys
			case EKeyCode::Escape: return "Escape";
			case EKeyCode::Enter: return "Enter";
			case EKeyCode::Space: return "Space";
			case EKeyCode::Backspace: return "Backspace";
			case EKeyCode::Tab: return "Tab";
			case EKeyCode::Shift: return "Shift";
			case EKeyCode::Ctrl: return "Ctrl";
			case EKeyCode::Alt: return "Alt";

				// Arrow keys
			case EKeyCode::Left: return "Left";
			case EKeyCode::Right: return "Right";
			case EKeyCode::Up: return "Up";
			case EKeyCode::Down: return "Down";

				// Function keys
			case EKeyCode::F1: return "F1";
			case EKeyCode::F2: return "F2";
			case EKeyCode::F3: return "F3";
			case EKeyCode::F4: return "F4";
			case EKeyCode::F5: return "F5";
			case EKeyCode::F6: return "F6";
			case EKeyCode::F7: return "F7";
			case EKeyCode::F8: return "F8";
			case EKeyCode::F9: return "F9";
			case EKeyCode::F10: return "F10";
			case EKeyCode::F11: return "F11";
			case EKeyCode::F12: return "F12";

			case EKeyCode::Count: return "Count";

			default: return "Unknown";
		}
	}
	std::string InputManager::GetActionAsString(const EInputAction& action)
	{
		switch (action)
		{
		case EInputAction::Press: return "Press";
		case EInputAction::Release: return "Release";
		case EInputAction::Down: return "Down";
		case EInputAction::None: return "Unknown";
		}

		return {};
	}
}//namespace Engine