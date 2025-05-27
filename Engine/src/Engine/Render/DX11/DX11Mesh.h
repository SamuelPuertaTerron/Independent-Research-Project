#pragma once
#include <d3d11.h>

#include "Engine/Render/Resources/IMesh.h"

#include "Engine/Render/DX11/DX11IndexBuffer.h"
#include "Engine/Render/DX11/DX11VertexBuffer.h"

#include "Engine/Render/DX11/DX11Vertex.h"
#include "Engine/Render/Vertext.h"

namespace Engine::Render::DX11
{
    class DX11Mesh : public IMesh
    {
    public:
        DX11Mesh(ID3D11Device* device, ID3D11DeviceContext* context)
	        : m_Device(device), m_Context(context) {}

        virtual ~DX11Mesh() override = default;

		virtual bool Init(const std::vector<Render::Vertex>& vertices, const std::vector<uint32_t>& indices = {}) override;

        virtual void Render() override;

        virtual void Destroy() override;

        size_t GetVertexCount() const { return m_VertexCount; }

        size_t GetIndexCount() const { return m_IndexCount; }

        bool IsIndexed() const { return m_IndexBuffer != nullptr; }
    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        std::unique_ptr<DX11VertexBuffer> m_VertexBuffer;
        std::unique_ptr<DX11IndexBuffer> m_IndexBuffer;
        size_t m_VertexCount = 0;
        size_t m_IndexCount = 0;
    };
}//namespace Engine::Render::DX11