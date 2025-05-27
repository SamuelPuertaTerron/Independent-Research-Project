/* Input Manager to handle input */
#pragma once

namespace Engine
{
	enum class EKeyCode : uint8_t;
	enum class EMouseButton : uint8_t;

	enum class EInputAction : uint8_t
	{
		None,
		Press,
		Release,
		Down,
	};

	struct Key
	{
		EKeyCode Code;
		EInputAction Action;
	};

	class InputManager
	{
	public:
		static void Init();
		static bool IsKeyDown(EKeyCode code);
		static bool IsKeyPressed(EKeyCode code);
		static bool IsKeyReleased(EKeyCode code);

		static void KeyDown(EKeyCode code, EInputAction action);
		static void KeyUp(EKeyCode code, EInputAction action);

		static bool IsMouseDown(EMouseButton button);

		static void MouseDown(EMouseButton button);
		static void MouseUp(EMouseButton button);

		static void CallKeyCallback(EKeyCode code, EInputAction action);
		static bool IsKeyCallbackValid();

		[[nodiscard]] static std::string GetKeyCodeAsString(const EKeyCode& code);
		[[nodiscard]] static std::string GetActionAsString(const EInputAction& action);

	private:
		inline static std::unordered_map<int, Key> m_Keys;
		inline static std::unordered_map<EMouseButton, bool> m_MouseButtons;
	};
}//namespace Engine