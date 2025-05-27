#include "Engine.h"
#include "ICameraContext.h"

#include "Engine/Render/DX11/DX11Camera.h"
#include "Engine/Render/OpenGL/OpenGLCamera.h"

namespace Engine::Render
{
	std::shared_ptr<ICameraContext> ICameraContext::Create()
	{

		switch (g_GraphicsAPI)
		{
		case EGraphicsAPI::DX11:
			return std::make_shared<DX11::DX11Camera>();
		case EGraphicsAPI::OpenGL:
			return std::make_shared<OpenGL::OpenGLCamera>();
		case EGraphicsAPI::None:
			break;
		}

		return nullptr;

	}
}//namespcae Engine::Render
