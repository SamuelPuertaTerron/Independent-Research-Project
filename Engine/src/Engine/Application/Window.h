/* The main window, encapsulates native windows like a Windows window. */

#pragma once

namespace Engine
{
	class INativeWindow;

	class Window
	{
	public:
		Window() = default;
		~Window() = default;

		[[nodiscard]] bool Init(const ApplicationSettings& settings);
		[[nodiscard]] bool LateInit() const;
		void Destroy() const;
		void EarlyDestroy() const;

		void PollEvents() const;
		/* Only for OpenGL */
		void SwapBuffers() const;

		void SetWindowSize(Vector2Int size);

		/* Getter/Setter functions */

		[[nodiscard]] Vector2Int GetWindowSize() const;

		[[nodiscard]] std::shared_ptr<INativeWindow> GetNativeWindow() const;

	private:
		std::shared_ptr<INativeWindow> m_NativeWindow;
		Vector2Int m_WindowSize;
	};

	const char* GetProcAddress(const char* name);
}//namespace Engine