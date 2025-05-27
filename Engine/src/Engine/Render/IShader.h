/* The base class for Shader implementations */
#pragma once

namespace Engine::Render
{
	class IRenderContext;

	/* The shader type. Can expand this if I include Geometry shaders. */
	enum class EShaderType
	{
		None = 0,
		Vertex,
		Fragment,
	};

	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Compile(IRenderContext* context, const fs::path& path) = 0;
		virtual void Destroy() = 0;

		//Optional for Graphics API
		virtual void Bind() {}
		virtual uint32_t GetShaderID() { return -1;  }

		static std::shared_ptr<IShader> Create(EShaderType type);
	};
}//namespace Engine::Render