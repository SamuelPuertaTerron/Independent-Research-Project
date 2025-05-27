#include "Engine.h"
#include "IImGuiWindow.h"

#include "Engine/Render/DX11/DX11ImGuiWindow.h"
#include "Engine/Render/OpenGL/OpenGLImGuiWindow.h"

namespace Engine
{
	std::shared_ptr<IImGuiWindow> IImGuiWindow::Create()
	{
		switch (Render::g_GraphicsAPI)
		{
		case Render::EGraphicsAPI::DX11:
			return std::make_unique<Render::DX11::DX11ImGuiWindow>();
		case Render::EGraphicsAPI::OpenGL:
			return std::make_unique<Render::OpenGL::OpenGLImGuiWindow>();
		case Render::EGraphicsAPI::None:
			break;
		}

		return nullptr;
	}
}//namespace Engine 
