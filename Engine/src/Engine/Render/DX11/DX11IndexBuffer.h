/* The Indice Buffer */
#pragma once
#include "DX11Buffer.h"

namespace Engine::Render::DX11
{
	class DX11IndexBuffer
	{
	public:
		DX11IndexBuffer() = default;
		~DX11IndexBuffer() = default;

		void Init(ID3D11Device* device, ID3D11DeviceContext* context, const std::vector<uint32_t>& indices);
		void Destroy() const;
		void Render(ID3D11DeviceContext* context);

		ID3D11Buffer* GetBuffer() const;

	private:
		ID3D11Buffer* m_pIndexBuffer;
		UINT m_IndexCount;
	};
}//namespace Engine::Render::DX11