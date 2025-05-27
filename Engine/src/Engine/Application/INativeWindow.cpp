#include "Engine.h"
#include "INativeWindow.h"

#include "Windows/WindowsWindow.h"

namespace Engine
{
	std::shared_ptr<INativeWindow> INativeWindow::Create()
	{
#ifdef _WIN32
		return std::make_shared<Win32::WindowsWindow>();
#endif
	}
}//namespace Engine