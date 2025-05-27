#include "Engine.h"
#include "DX11Mesh.h"

#include "DX11Vertex.h"

namespace Engine::Render::DX11
{
    bool DX11Mesh::Init(const std::vector<Render::Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        m_VertexCount = vertices.size();
        m_IndexCount = indices.size();

        m_VertexBuffer = std::make_unique<DX11VertexBuffer>();
        m_VertexBuffer->Init(m_Device, m_Context, ConvertFromVertexArray(vertices));

        if (!indices.empty())
        {
            m_IndexBuffer = std::make_unique<DX11IndexBuffer>();
            m_IndexBuffer->Init(m_Device, m_Context, indices);
        }

        return true;
    }

    void DX11Mesh::Render()
    {
        auto vBuffer = m_VertexBuffer->GetBuffer();

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        m_Context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

        if (m_IndexBuffer)
        {
            m_Context->IASetIndexBuffer(m_IndexBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
            m_Context->DrawIndexed(m_IndexCount, 0, 0);
        }
        else
        {
            m_Context->Draw(m_VertexCount, 0);
        }
    }

    void DX11Mesh::Destroy()
    {
        if (m_VertexBuffer) m_VertexBuffer->Destroy();
        if (m_IndexBuffer) m_IndexBuffer->Destroy();
    }
}//namespace Engine::Render::DX11