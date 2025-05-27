/* DirectX buffer, with custom flags */

#pragma once
#include <d3d11.h>

namespace Engine::Render::DX11
{
	class DX11ConstantBuffer
	{
	public:
		DX11ConstantBuffer() = default;
		~DX11ConstantBuffer() = default;

		template<typename T, size_t Amount = 1>
		bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
		{
			D3D11_BUFFER_DESC cbDesc;
			ZeroMemory(&cbDesc, sizeof(cbDesc));
			cbDesc.Usage = D3D11_USAGE_DEFAULT;
			cbDesc.ByteWidth = sizeof(T) * Amount;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = 0;
			HRESULT hr = device->CreateBuffer(&cbDesc, nullptr, &m_pBuffer);
			if (FAILED(hr))
				return false;

			deviceContext->VSSetConstantBuffers(0, 1, &m_pBuffer);
			deviceContext->PSSetConstantBuffers(0, 1, &m_pBuffer);

			return true;
		}
		void Destroy() const;

		void Bind(ID3D11DeviceContext* deviceContext) const
		{
			deviceContext->VSSetConstantBuffers(0, 1, &m_pBuffer); // Re-bind after update
		}

		ID3D11Buffer* GetBuffer() const
		{
			return m_pBuffer;
		}

	private:
		ID3D11Buffer* m_pBuffer;
	};

}//namespace Engine::Render::DX11