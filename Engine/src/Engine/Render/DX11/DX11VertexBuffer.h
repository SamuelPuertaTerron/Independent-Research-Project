/* The DX11 vertex buffer class */
#pragma once
#include "DX11Buffer.h"
#include "DX11Vertex.h"

namespace Engine::Render::DX11
{
	struct DX11Vertex;

	class DX11VertexBuffer
	{
	public:
		DX11VertexBuffer() = default;
		~DX11VertexBuffer() = default;

		void Init(ID3D11Device* device, ID3D11DeviceContext* context, const std::vector<DX11Vertex>& vertices);
		void Destroy() const;
		void Render(ID3D11DeviceContext* context);

		ID3D11Buffer* GetBuffer();

	private:
		ID3D11Buffer* m_pVertexBuffer;
		UINT m_VertexCount;
	};
}//namespace Engine::Render::DX11