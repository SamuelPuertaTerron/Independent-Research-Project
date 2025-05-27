/* The Shader that runs after Rasterization stage. The same as OpenGL Fragment shader */
#pragma once
#include "DX11VertexShader.h"
#include "../IShader.h"

namespace Engine::Render::DX11
{
	class DX11PixelShader : public IShader
	{
	public:
		DX11PixelShader() = default;
		virtual ~DX11PixelShader() override = default;

		virtual void Compile(IRenderContext* context, const fs::path& path) override;
		virtual void Destroy() override;

		void Bind(ID3D11DeviceContext* context) const;

	private:
		/* Compiles the preloaded shader built-in to the application */
		ID3DBlob* CompileHardcodedShader(ID3DBlob* blob) const;
	private:
		ID3D11PixelShader* m_pShader;
	};
}//namespace Engine::Render::DX11