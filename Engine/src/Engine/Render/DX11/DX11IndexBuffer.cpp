#include "Engine.h"
#include "DX11IndexBuffer.h"

namespace Engine::Render::DX11
{
    void DX11IndexBuffer::Init(ID3D11Device* device, ID3D11DeviceContext* context, const std::vector<uint32_t>& indices)
    {
        D3D11_BUFFER_DESC ibd;
        ZeroMemory(&ibd, sizeof(ibd));
        ibd.Usage = D3D11_USAGE_DEFAULT;
        ibd.ByteWidth = sizeof(UINT32) * static_cast<UINT>(indices.size());
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(data));
        data.pSysMem = indices.data();

        device->CreateBuffer(&ibd, &data, &m_pIndexBuffer);

        // Specify the correct format for 32-bit indices
        context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    }

	void DX11IndexBuffer::Destroy() const
	{
		m_pIndexBuffer->Release();
	}

	void DX11IndexBuffer::Render(ID3D11DeviceContext* context)
	{

	}

	ID3D11Buffer* DX11IndexBuffer::GetBuffer() const
	{
		return m_pIndexBuffer != nullptr ? m_pIndexBuffer : nullptr;
	}
}//namespace Engine::Render::DX11