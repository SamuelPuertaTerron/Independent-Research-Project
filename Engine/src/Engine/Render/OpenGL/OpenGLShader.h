/* The main shader for OpenGL */
#pragma once
#include "Engine/Render/IShader.h"

namespace Engine::Render::OpenGL
{
	class OpenGLShader : public IShader
	{
	public:
		OpenGLShader() = default;
		virtual ~OpenGLShader() override = default;

		virtual void Compile(IRenderContext* context, const fs::path& path) override;
		virtual void Destroy() override;
		virtual void Bind() override;

		virtual uint32_t GetShaderID() override
		{
			return m_ShaderID;
		}

	private:
		void ExtractShaders(const std::string& fileContent, std::string& vertexShader, std::string& fragmentShader, std::string& tessControl, std::string& tessEval, std::string& geometryShader);
	private:
		uint32_t m_ShaderID;
	};

}//namespace Engine::Render::OpenGL