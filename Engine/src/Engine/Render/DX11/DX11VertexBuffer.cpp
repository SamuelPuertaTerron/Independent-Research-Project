#include "Engine.h"
#include "DX11VertexBuffer.h"
#include "DX11VertexBuffer.h"

#include "Engine/Render/Vertext.h"

namespace Engine::Render::DX11
{
	void DX11VertexBuffer::Init(ID3D11Device* device, ID3D11DeviceContext* context, const std::vector<DX11Vertex>& vertices)
	{
		m_VertexCount = { static_cast<UINT>(vertices.size()) };

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * m_VertexCount;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices.data();
		if (FAILED(device->CreateBuffer(&vbd, &vinitData, &m_pVertexBuffer)))
		{
			SE_ASSERT(false, "Failed to create DX11 Vertex Buffer!");
			return;
		}

		//Bind the vertex buffer to the input assembler
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	}

	void DX11VertexBuffer::Destroy() const
	{
		m_pVertexBuffer->Release();
	}

	void DX11VertexBuffer::Render(ID3D11DeviceContext* context)
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	}

	ID3D11Buffer* DX11VertexBuffer::GetBuffer()
	{
		if (m_pVertexBuffer)
			return m_pVertexBuffer;

		return nullptr;
	}
}//namespace Engine::Render::DX11