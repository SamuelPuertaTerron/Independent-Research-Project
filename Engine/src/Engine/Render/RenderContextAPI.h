/* A set of helper functions for the currently selected API */
#pragma once

namespace Engine::Render
{
	enum class EGraphicsAPI
	{
		None = 0,
		OpenGL = 1,
		DX11 = 2
	};

	inline EGraphicsAPI g_GraphicsAPI = EGraphicsAPI::None;

	inline std::string GraphicsAPIToString()
	{
		switch (g_GraphicsAPI)
		{
		case EGraphicsAPI::DX11:
			return "Direct3D11";
		case EGraphicsAPI::OpenGL:
			return "OpenGL";
		default:
			return "NULL";
		}

		return "NULL";
	}
}//namespace Engine::Render
