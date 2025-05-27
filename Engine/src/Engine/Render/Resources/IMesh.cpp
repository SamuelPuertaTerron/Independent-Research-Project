#include "Engine.h"
#include "IMesh.h"

#include "Engine/Render/Render.h"
#include "Engine/Render/DX11/DX11Mesh.h"
#include "Engine/Render/DX11/DX11RenderContext.h"
#include "Engine/Render/OpenGL/OpenGLMesh.h"


namespace Engine::Render
{
	std::shared_ptr<IMesh> IMesh::Create()
	{
		switch (g_GraphicsAPI)
		{
		case EGraphicsAPI::DX11:
		{
			DX11::DX11RenderContext* rc = dynamic_cast<DX11::DX11RenderContext*>(Renderer::GetRenderContext().get());
			ID3D11Device* device = rc->GetDevice();
			ID3D11DeviceContext* context = rc->GetImmediateContext();
			return std::make_shared<DX11::DX11Mesh>(device, context);
		}
		case EGraphicsAPI::OpenGL:
			return std::make_shared<OpenGL::OpenGLMesh>();
		case EGraphicsAPI::None:
			return nullptr;
		}

		return nullptr;
	}
}//namespace Engine::Render