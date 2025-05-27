#include "Engine.h"
#include "DX11RenderContext.h"

#include <DirectXMath.h>
#include <wrl/client.h>

#include "DX11Buffer.h"
#include "DX11Camera.h"
#include "DX11IndexBuffer.h"
#include "DX11Mesh.h"
#include "DX11PixelShader.h"
#include "DX11VertexShader.h"
#include "DX11Vertex.h"
#include "DX11VertexBuffer.h"

#include "Engine/Application/Window.h"
#include "Engine/Application/INativeWindow.h"
#include "Engine/Render/Resources/Camera.h"

using namespace DirectX;

namespace Engine::Render::DX11
{
    struct TransformBuffer
    {
        XMFLOAT4X4 world;
        XMFLOAT4X4 view;
        XMFLOAT4X4 projection;
        XMFLOAT4 color;
    };

    bool DX11RenderContext::Init()
    {
        HRESULT hr = S_OK;
        HWND handle = static_cast<HWND>(Application::GetInstance().GetWindow()->GetNativeWindow()->GetWindowHandle());

        RECT rc;
        GetClientRect(handle, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        // Set up the Swap chain description
        DXGI_SWAP_CHAIN_DESC scd;
        ZeroMemory(&scd, sizeof(scd));
        scd.BufferCount = 1;
        scd.BufferDesc.Width = width;
        scd.BufferDesc.Height = height;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.RefreshRate.Numerator = 60;
        scd.BufferDesc.RefreshRate.Denominator = 1;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = handle;
        scd.SampleDesc.Count = 1;  // Swap chain always uses 1 sample when using MSAA render target
        scd.SampleDesc.Quality = 0;
        scd.Windowed = TRUE;

        D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        D3D_FEATURE_LEVEL featureLevel;

        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,
            featureLevels,
            1,
            D3D11_SDK_VERSION,
            &scd,
            &m_pSwapChain,
            &m_pD3dDevice,
            &featureLevel,
            &m_pImmediateContext
        );

        if (FAILED(hr))
            return false;

        // Check MSAA support
        UINT sampleCount = 4;
        UINT msaaQuality = 0;
        hr = m_pD3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &msaaQuality);
        if (FAILED(hr) || msaaQuality == 0) {
            sampleCount = 1;
            msaaQuality = 0;
        }

        // Create MSAA render target
        D3D11_TEXTURE2D_DESC rtDesc = {};
        rtDesc.Width = width;
        rtDesc.Height = height;
        rtDesc.MipLevels = 1;
        rtDesc.ArraySize = 1;
        rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        rtDesc.SampleDesc.Count = sampleCount;
        rtDesc.SampleDesc.Quality = (sampleCount > 1) ? (msaaQuality - 1) : 0;
        rtDesc.Usage = D3D11_USAGE_DEFAULT;
        rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
        hr = m_pD3dDevice->CreateTexture2D(&rtDesc, nullptr, &m_pMSAARenderTarget);
        if (FAILED(hr)) return false;

        // Create MSAA render target view
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = rtDesc.Format;
        rtvDesc.ViewDimension = (sampleCount > 1) ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
        hr = m_pD3dDevice->CreateRenderTargetView(m_pMSAARenderTarget, &rtvDesc, &m_pMSAARenderTargetView);
        if (FAILED(hr)) return false;

        // Create rasterizer state with MSAA support
        D3D11_RASTERIZER_DESC rasterDesc = {};
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.CullMode = D3D11_CULL_BACK;
        rasterDesc.FrontCounterClockwise = false;
        rasterDesc.DepthBias = 0;
        rasterDesc.DepthBiasClamp = 0.0f;
        rasterDesc.SlopeScaledDepthBias = 0.0f;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.ScissorEnable = false;
        rasterDesc.MultisampleEnable = (sampleCount > 1);
        rasterDesc.AntialiasedLineEnable = false;

        hr = m_pD3dDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);
        if (FAILED(hr)) return false;
        m_pImmediateContext->RSSetState(m_pRasterState);

        m_SampleCount = sampleCount;
        m_MsaaQuality = msaaQuality;

        // Create depth stencil (now with MSAA support)
        CreateDepthStencil();

        // Create back buffer RTV for final presentation
        ID3D11Texture2D* backBuffer = nullptr;
        hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
        if (FAILED(hr)) return false;

        hr = m_pD3dDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pBackBufferRTV);
        backBuffer->Release();
        if (FAILED(hr)) return false;

        // Bind MSAA render target and depth stencil
        m_pImmediateContext->OMSetRenderTargets(1, &m_pMSAARenderTargetView, m_pDepthStencilView);

        // Set viewport
        D3D11_VIEWPORT vp = {};
        vp.Width = static_cast<FLOAT>(width);
        vp.Height = static_cast<FLOAT>(height);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        m_pImmediateContext->RSSetViewports(1, &vp);

        // Rest of initialization (shaders, geometry, etc.)
        m_VertexShader = std::make_shared<DX11VertexShader>();
        m_VertexShader->Compile(this, "Resources/VertexShader.hlsl");

        m_PixelShader = std::make_shared<DX11PixelShader>();
        m_PixelShader->Compile(this, "Resources/PixelShader.hlsl");

        m_ConstantBuffer = std::make_shared<DX11ConstantBuffer>();
        m_ConstantBuffer->Init<TransformBuffer>(m_pD3dDevice, m_pImmediateContext);

        CreateDefaultGeometry();

        Logger::Log("Created DX11 Graphics Backend", ELogLevel::Info);
        return true;
    }

    void DX11RenderContext::CreateDepthStencil()
    {
        HWND handle = static_cast<HWND>(Application::GetInstance().GetWindow()->GetNativeWindow()->GetWindowHandle());
        RECT rc;
        GetClientRect(handle, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc.SampleDesc.Count = m_SampleCount;
        desc.SampleDesc.Quality = (m_SampleCount > 1) ? (m_MsaaQuality - 1) : 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        HRESULT hr = m_pD3dDevice->CreateTexture2D(&desc, nullptr, &m_pDepthStencilBuffer);
        if (FAILED(hr))
        {
            // Add error information to your assert
            SE_ASSERT(false, "Failed to create depth stencil buffer. HRESULT:  " + std::to_string(hr));
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC descStencil;
        ZeroMemory(&descStencil, sizeof(descStencil));
        descStencil.Format = desc.Format;
        descStencil.ViewDimension = (m_SampleCount > 1) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
        descStencil.Texture2D.MipSlice = 0;
        hr = m_pD3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &descStencil, &m_pDepthStencilView);
        if (FAILED(hr))
        {
            SE_ASSERT(false, "Failed to create depth stencil view");
        }
    }

    void DX11RenderContext::CreateDefaultGeometry()
    {
        // Define vertices for a cube
        std::vector<DX11Vertex> vertices = 
        {
            // Front face
            { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
            { XMFLOAT3(0.5f,  0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
            { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },

            // Back face
            { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
            { XMFLOAT3(-0.5f,  0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
            { XMFLOAT3(0.5f,  0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
            { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f) }
        };

        // Define indices for the cube (two triangles per face)
        std::vector<UINT> indices = {
            // Front face
            0, 1, 2,
            0, 2, 3,

            // Back face
            4, 6, 5,
            4, 7, 6,

            // Left face
            4, 5, 1,
            4, 1, 0,

            // Right face
            3, 2, 6,
            3, 6, 7,

            // Top face
            1, 5, 6,
            1, 6, 2,

            // Bottom face
            4, 0, 3,
            4, 3, 7
        };

        // Create vertex buffer
        m_VertexBuffer = std::make_shared<DX11VertexBuffer>();
        m_VertexBuffer->Init(m_pD3dDevice, m_pImmediateContext, vertices);

        // Create index buffer
        m_IndexBuffer = std::make_shared<DX11IndexBuffer>();
        m_IndexBuffer->Init(m_pD3dDevice, m_pImmediateContext, indices);
    }

    void DX11RenderContext::Destroy()
    {
        if (m_ConstantBuffer) m_ConstantBuffer->Destroy();
        if (m_VertexShader) m_VertexShader->Destroy();
        if (m_PixelShader) m_PixelShader->Destroy();

        if (m_pDepthStencilView) m_pDepthStencilView->Release();
        if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
        if (m_pMSAARenderTargetView) m_pMSAARenderTargetView->Release();
        if (m_pMSAARenderTarget) m_pMSAARenderTarget->Release();
        if (m_pBackBufferRTV) m_pBackBufferRTV->Release();
    	if (m_pSwapChain) m_pSwapChain->Release();
        if (m_pImmediateContext) m_pImmediateContext->Release();
        if (m_pD3dDevice) m_pD3dDevice->Release();

        Logger::Log("Destroyed DX11 Graphics Backend", ELogLevel::Info);
    }

    void DX11RenderContext::BeginRender(const std::shared_ptr<Render::Camera>& camera)
    {
        camera->UpdateTransform();

        DX11Camera* cam = static_cast<DX11Camera*>(camera->GetContext().get());
        
       m_RenderData.View = cam->GetViewMatrix();
       m_RenderData.Projection = cam->GetProjectionMatrix();

        // Clear MSAA render target AND depth buffer
        const float clearColor[4] = { 0.1f, 0.1f, 0.2f, 1.0f };
        m_pImmediateContext->ClearRenderTargetView(m_pMSAARenderTargetView, clearColor);
        m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        // Verify render targets are properly set
        m_pImmediateContext->OMSetRenderTargets(1, &m_pMSAARenderTargetView, m_pDepthStencilView);
    }

    void DX11RenderContext::EndRender()
	{
        // Resolve MSAA render target to back buffer
        ID3D11Texture2D* backBufferTex = nullptr;
        if (SUCCEEDED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTex))) 
        {
            m_pImmediateContext->ResolveSubresource(
                backBufferTex, 0,
                m_pMSAARenderTarget, 0,
                DXGI_FORMAT_R8G8B8A8_UNORM
            );

            backBufferTex->Release();
        }
        m_pSwapChain->Present(1, 0);
    }

    void DX11RenderContext::DrawQuad(Vector3 position, Vector3 rotation, Vector3 scale, Colour colour)
    {
        UpdateTransform(position, rotation, scale, colour);

        // Set vertex buffer
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        ID3D11Buffer* vertexBuffer = m_VertexBuffer->GetBuffer();
        m_pImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

        // Set index buffer
        m_pImmediateContext->IASetIndexBuffer(m_IndexBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

        // Set primitive topology
        m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Draw the cube
        m_pImmediateContext->DrawIndexedInstanced(36, 2, 0, 0, 0);
    }

    void DX11RenderContext::Draw(const std::shared_ptr<IMesh>& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour)
    {
        if (!mesh) return;

        UpdateTransform(position, rotation, scale, colour);

        DX11Mesh* dx11Mesh = dynamic_cast<DX11Mesh*>(mesh.get());
        dx11Mesh->Render();
    }

    void DX11RenderContext::ResizeViewport(const Vector2Int& size)
    {
        // Ensure we have valid device and swap chain
        if (!m_pD3dDevice || !m_pSwapChain || size.X <= 0 || size.Y <= 0)
            return;

        // Release existing resources
        if (m_pMSAARenderTargetView) m_pMSAARenderTargetView->Release();
        if (m_pMSAARenderTarget) m_pMSAARenderTarget->Release();
        if (m_pDepthStencilView) m_pDepthStencilView->Release();
        if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
        if (m_pBackBufferRTV) m_pBackBufferRTV->Release();

        // Resize swap chain buffers
        HRESULT hr = m_pSwapChain->ResizeBuffers(
            1,
            static_cast<UINT>(size.X),
            static_cast<UINT>(size.Y),
            DXGI_FORMAT_R8G8B8A8_UNORM,
            0
        );

        if (FAILED(hr)) {
            Logger::Log("Failed to resize swap chain buffers", ELogLevel::Error);
            return;
        }

        // Recreate back buffer RTV
        ID3D11Texture2D* pBackBuffer = nullptr;
        hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
        if (SUCCEEDED(hr)) {
            hr = m_pD3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pBackBufferRTV);
            pBackBuffer->Release();
        }

        // Recreate MSAA render target
        D3D11_TEXTURE2D_DESC rtDesc = {};
        rtDesc.Width = size.X;
        rtDesc.Height = size.Y;
        rtDesc.MipLevels = 1;
        rtDesc.ArraySize = 1;
        rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        rtDesc.SampleDesc.Count = m_SampleCount;
        rtDesc.SampleDesc.Quality = (m_SampleCount > 1) ? (m_MsaaQuality - 1) : 0;
        rtDesc.Usage = D3D11_USAGE_DEFAULT;
        rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET;

        hr = m_pD3dDevice->CreateTexture2D(&rtDesc, nullptr, &m_pMSAARenderTarget);
        if (FAILED(hr)) return;

        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = rtDesc.Format;
        rtvDesc.ViewDimension = (m_SampleCount > 1) ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
        hr = m_pD3dDevice->CreateRenderTargetView(m_pMSAARenderTarget, &rtvDesc, &m_pMSAARenderTargetView);

        // Recreate depth stencil
        D3D11_TEXTURE2D_DESC depthDesc = {};
        depthDesc.Width = size.X;
        depthDesc.Height = size.Y;
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.SampleDesc.Count = m_SampleCount;
        depthDesc.SampleDesc.Quality = (m_SampleCount > 1) ? (m_MsaaQuality - 1) : 0;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = m_pD3dDevice->CreateTexture2D(&depthDesc, nullptr, &m_pDepthStencilBuffer);
        if (FAILED(hr)) return;

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = depthDesc.Format;
        dsvDesc.ViewDimension = (m_SampleCount > 1) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;
        hr = m_pD3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &dsvDesc, &m_pDepthStencilView);

        // Update viewport
        D3D11_VIEWPORT vp;
        vp.Width = static_cast<FLOAT>(size.X);
        vp.Height = static_cast<FLOAT>(size.Y);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        m_pImmediateContext->RSSetViewports(1, &vp);

        // Rebind render targets
        m_pImmediateContext->OMSetRenderTargets(1, &m_pMSAARenderTargetView, m_pDepthStencilView);
    }

    void DX11RenderContext::UpdateTransform(const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour)
    {
        // Create world matrix
        XMMATRIX worldMatrix = XMMatrixScaling(scale.X, scale.Y, scale.Z) *
            XMMatrixRotationRollPitchYaw(
                XMConvertToRadians(rotation.X),
                XMConvertToRadians(rotation.Y),
                XMConvertToRadians(rotation.Z)) *
            XMMatrixTranslation(position.X, position.Y, position.Z);

        // Prepare constant buffer data
        TransformBuffer bufferData;
        XMStoreFloat4x4(&bufferData.world, XMMatrixTranspose(worldMatrix));
        bufferData.color = XMFLOAT4(colour.R, colour.G, colour.B, colour.A);
        XMStoreFloat4x4(&bufferData.view, XMMatrixTranspose(XMLoadFloat4x4(&m_RenderData.View)));
        XMStoreFloat4x4(&bufferData.projection, XMMatrixTranspose(XMLoadFloat4x4(&m_RenderData.Projection)));

        // Update constant buffer
        m_pImmediateContext->UpdateSubresource(
            m_ConstantBuffer->GetBuffer(),
            0,
            nullptr,
            &bufferData,
            0,
            0
        );

        // Set shaders and constant buffer
        m_VertexShader->Bind(m_pImmediateContext);
        m_PixelShader->Bind(m_pImmediateContext);
        m_ConstantBuffer->Bind(m_pImmediateContext);
    }
}