#include "Engine.h"
#include "IShader.h"

#include "DX11/DX11PixelShader.h"
#include "OpenGL/OpenGLShader.h"

namespace Engine::Render
{
	/* Currently only supports DX11, but can expand this to include more Graphics APIs */
	std::shared_ptr<IShader> IShader::Create(EShaderType type)
	{
		switch (g_GraphicsAPI)
		{
		case EGraphicsAPI::DX11:
			{
				switch (type)
				{
				case EShaderType::Fragment:
					return std::make_shared<DX11::DX11PixelShader>();
				case EShaderType::Vertex:
					return std::make_shared<DX11::DX11VertexShader>();
				case EShaderType::None:
					return nullptr;
				}
			}
		case EGraphicsAPI::OpenGL:
			return std::make_shared<OpenGL::OpenGLShader>();
		case EGraphicsAPI::None:
			break;
		}

		return nullptr;
	}
}//namespace Engine::Render