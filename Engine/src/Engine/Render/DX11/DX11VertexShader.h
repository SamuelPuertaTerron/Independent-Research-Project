/* A shader that is ran after the Input-Assembler stage */
#pragma once
#include <d3d11.h>

#include "../IShader.h"

namespace Engine::Render::DX11
{
	class DX11VertexShader : public IShader
	{
	public:
		DX11VertexShader() = default;
		virtual ~DX11VertexShader() override = default;

		virtual void Compile(IRenderContext* context, const fs::path& path) override;
		virtual void Destroy() override;

		void Bind(ID3D11DeviceContext* context) const;

		void ReleaseBlob(ID3D11DeviceContext* immediateContext) const;

	private:
		ID3DBlob* CompileHardcodedShader(ID3DBlob* blob) const;
	private:
		ID3D11VertexShader* m_pShader;
		ID3D11InputLayout* m_pInputLayout;
		ID3DBlob* m_Blob;
	};

}//namespace Engine::Render::DX11