/* A vertex for DX11 */
#pragma once
#include <DirectXMath.h>

//TODO: Make this work with the other Vertex struct

#include "Engine/Render/Vertext.h"

namespace Engine::Render::DX11
{
	struct DX11Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT3 Normal;
	};

	inline DX11Vertex ConvertFromVertex(const Render::Vertex& vertex)
	{
		DX11Vertex vert;
		vert.Position = { vertex.Position.X, vertex.Position.Y, vertex.Position.Z };
		vert.Normal = { vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z };
		vert.Color = { vertex.Colour.R, vertex.Colour.G, vertex.Colour.B, vertex.Colour.A };
		return vert;
	}

	inline std::vector<DX11Vertex> ConvertFromVertexArray(const std::vector<Render::Vertex>& vertices)
	{
		std::vector<DX11Vertex> dx11Vertices;

		for (const auto& vertex : vertices)
		{
			DX11Vertex vert;
			vert.Position = { vertex.Position.X, vertex.Position.Y, vertex.Position.Z };
			vert.Normal = { vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z };
			vert.Color = { vertex.Colour.R, vertex.Colour.G, vertex.Colour.B, vertex.Colour.A };
			dx11Vertices.push_back(vert);
		}

		return dx11Vertices;
	}

}//namespace Engine::Render::DX11
