/* A native window interface to be used platforms. */
#pragma once

namespace Engine
{
	class INativeWindow
	{
	public:
		virtual ~INativeWindow() = default;

		/* Gets called when the window its created */
		virtual bool Init() = 0;
		/* Gets called after all major systems have been created and initialized */
		virtual bool LateInit() = 0;

		/* Gets called when the window is destroyed */
		virtual void Destroy() = 0;
		/* Gets called before any other system is destroyed */
		virtual void EarlyDestroy() = 0;

		/* Handle Window Events */
		virtual void PollEvents() = 0;

		/* Switches the rendering buffers */
		virtual void SwapBuffers() = 0;

		/* Returns the OS specific Window Handle */
		virtual void* GetWindowHandle() = 0;

		virtual void* GetProcAddress(const char* name) = 0;

		/* Factory Method to create a Native Window for example Windows window */
		static std::shared_ptr<INativeWindow> Create();
	};

}//namespace Engine