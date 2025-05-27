#include "Engine.h"
#include "DX11PixelShader.h"

#include <d3dcompiler.h>

#include "DX11RenderContext.h"

namespace Engine::Render::DX11
{
    void DX11PixelShader::Compile(IRenderContext* context, const fs::path& path)
    {
        HRESULT hr = S_OK;
        ID3DBlob* psBlob = nullptr;

        // First try to compile from file if path exists
        if (fs::exists(path))
        {
            std::wstring source = path.wstring();
            hr = D3DCompileFromFile(source.c_str(), nullptr, nullptr, "PS", "ps_5_0", 0, 0, &psBlob, nullptr);
        }

        // If file compilation failed or file doesn't exist, use fallback
        if (FAILED(hr) || psBlob == nullptr)
        {
            psBlob = CompileHardcodedShader(psBlob);
        }

        auto renderContext = dynamic_cast<DX11RenderContext*>(context);
        SE_ASSERT(renderContext, "Cannot create shader as Render Context is not available");
        auto device = renderContext->GetDevice();

        hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pShader);

        // Release the shader blob
        if (psBlob) psBlob->Release();

        auto immediateContext = renderContext->GetImmediateContext();
        immediateContext->PSSetShader(m_pShader, nullptr, 0);
    }

    void DX11PixelShader::Destroy()
    {
		m_pShader->Release();
    }

    void DX11PixelShader::Bind(ID3D11DeviceContext* context) const
    {
        if (!context || !m_pShader)
            return;

        context->PSSetShader(m_pShader, nullptr, 0);
    }

    ID3DBlob* DX11PixelShader::CompileHardcodedShader(ID3DBlob* blob) const
    {
        const char* psSource = R"(
        struct PS_INPUT
        {
            float4 Pos : SV_POSITION;
            float4 Color : COLOR;
        };
        float4 PS(PS_INPUT input) : SV_Target
        {
            return input.Color;
        }
    )";

        HRESULT hr = D3DCompile(psSource, strlen(psSource), "pixelshader.hlsl", nullptr, nullptr, "PS", "ps_4_0", 0, 0, &blob, nullptr);
        if (FAILED(hr))
        {
            SE_ASSERT(false, "Could not compile pixel shader");
        }

        return blob;
    }
}//namespace Engine::Render::DX11