#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "DX11Mesh.h"
#include "../IRenderContext.h"

namespace Engine::Render
{
    class IShader;
}

namespace Engine::Render::DX11
{
    class DX11IndexBuffer;
    class DX11ConstantBuffer;
    class DX11VertexShader;
    class DX11PixelShader;
    class DX11VertexBuffer;

    struct RenderData
    {
	    DirectX::XMFLOAT4X4 View;
	    DirectX::XMFLOAT4X4 Projection;
    };

    class DX11RenderContext : public IRenderContext
    {
    public:
        DX11RenderContext() = default;
        virtual ~DX11RenderContext() override = default;

        [[nodiscard]] virtual bool Init() override;
        virtual void Destroy() override;

        virtual void BeginRender(const std::shared_ptr<Render::Camera>& camera) override;
        virtual void EndRender() override;

        virtual void DrawQuad(Vector3 position, Vector3 rotation, Vector3 scale, Colour colour) override;
        virtual void Draw(const std::shared_ptr<IMesh>& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour) override;

        virtual void ResizeViewport(const Vector2Int& size) override;

        [[nodiscard]] ID3D11Device* GetDevice() const { return m_pD3dDevice; }
        [[nodiscard]] ID3D11DeviceContext* GetImmediateContext() const { return m_pImmediateContext; }

    private:
        void CreateDepthStencil();
        void CreateDefaultGeometry();
        void UpdateTransform(const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour);

    private:
        ID3D11Device* m_pD3dDevice = nullptr;
        ID3D11DeviceContext* m_pImmediateContext = nullptr;
        IDXGISwapChain* m_pSwapChain = nullptr;
        ID3D11Texture2D* m_pMSAARenderTarget;
        ID3D11RenderTargetView* m_pMSAARenderTargetView;
        ID3D11RenderTargetView* m_pBackBufferRTV; // Renamed from m_pRenderTargetView
    	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
        ID3D11Texture2D* m_pDepthStencilBuffer = nullptr;
        ID3D11RasterizerState* m_pRasterState = nullptr;

        // Buffers
        std::shared_ptr<DX11VertexBuffer> m_VertexBuffer;
        std::shared_ptr<DX11IndexBuffer> m_IndexBuffer;

        // Shaders and constant buffer
        std::shared_ptr<DX11ConstantBuffer> m_ConstantBuffer;
        std::shared_ptr<DX11VertexShader> m_VertexShader;
        std::shared_ptr<DX11PixelShader> m_PixelShader;

        UINT m_SampleCount;
        UINT m_MsaaQuality;

        RenderData m_RenderData;
    };
}