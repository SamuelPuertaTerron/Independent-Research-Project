#include "Engine.h"
#include "DX11VertexShader.h"

#include <d3dcompiler.h>

#include "DX11RenderContext.h"

namespace Engine::Render::DX11
{
	void DX11VertexShader::Compile(IRenderContext* context, const fs::path& path)
	{
		HRESULT hr = S_OK;
		std::string str = path.string();
		const std::wstring source = std::wstring(str.begin(), str.end());
		hr = D3DCompileFromFile(source.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, &m_Blob, nullptr);
		//If the shader failed to compile from file, then go to fallback shader.
		if (FAILED(hr))
		{
			m_Blob = CompileHardcodedShader(m_Blob);
		}

		auto renderContext = dynamic_cast<DX11RenderContext*>(context);
		SE_ASSERT(renderContext, "Cannot create shader as Render Context is not available");
		auto device = renderContext->GetDevice();

		hr = device->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_pShader);
		if (SUCCEEDED(hr))
		{
			// Define the input layout (matches vertex structure)
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE(layout);
			hr = device->CreateInputLayout(layout, numElements, m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), &m_pInputLayout);
			if (FAILED(hr))
			{
				SE_ASSERT(false, "Could not create the layout");
			}
		}
	}

	void DX11VertexShader::Destroy()
	{
		m_pInputLayout->Release();
		m_pShader->Release();
	}

	void DX11VertexShader::Bind(ID3D11DeviceContext* context) const
	{
		if (!context || !m_pShader || !m_pInputLayout)
			return;

		context->VSSetShader(m_pShader, nullptr, 0);
		context->IASetInputLayout(m_pInputLayout);
	}

	void DX11VertexShader::ReleaseBlob(ID3D11DeviceContext* immediateContext) const
	{
		m_Blob->Release();

		immediateContext->VSSetShader(m_pShader, nullptr, 0);
		immediateContext->IASetInputLayout(m_pInputLayout);
	}

	ID3DBlob* DX11VertexShader::CompileHardcodedShader(ID3DBlob* blob) const
	{
		const char* vsSource = R"(
        struct VS_INPUT
        {
            float3 Pos : POSITION;
            float4 Color : COLOR;
        };
        struct PS_INPUT
        {
            float4 Pos : SV_POSITION;
            float4 Color : COLOR;
        };
        cbuffer ConstantBuffer : register(b0)
        {
            matrix World;
            matrix View;
            matrix Projection;
        };
        PS_INPUT VS(VS_INPUT input)
        {
            PS_INPUT output;
            output.Pos = mul(float4(input.Pos, 1.0), World);
            output.Pos = mul(output.Pos, View);
            output.Pos = mul(output.Pos, Projection);
            output.Color = input.Color;
            return output;
        }
    )";

		HRESULT hr = D3DCompile(vsSource, strlen(vsSource), "vertexshader.hlsl", nullptr, nullptr, "VS", "vs_4_0", 0, 0, &blob, nullptr);
		if (FAILED(hr))
		{
			SE_ASSERT(false, "Could not compile vertex shader");
		}

		return blob;
	}
}//namespace Engine::Render::DX11