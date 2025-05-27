#include "Engine.h"
#include "IRenderContext.h"

#include "DX11/DX11RenderContext.h"
#include "OpenGL/OpenGLRenderContext.h"

namespace Engine::Render
{
	std::shared_ptr<IRenderContext> IRenderContext::Create()
	{
		switch (g_GraphicsAPI)
		{
		case EGraphicsAPI::DX11:
			return std::make_shared<DX11::DX11RenderContext>();
		case EGraphicsAPI::OpenGL:
			return std::make_shared<OpenGL::OpenGLRenderContext>();
		case EGraphicsAPI::None:
			break;
		}

		return nullptr;
	}
}//namespace Engine::Render
