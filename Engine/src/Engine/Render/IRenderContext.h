/* The base render context (Graphics API) */
#pragma once

namespace Engine::Render
{
	class Camera;

	namespace DX11
	{
		class DX11Mesh;
	}

	class IMesh;
}

namespace Engine::Render
{
	class IRenderContext
	{
	public:
		virtual ~IRenderContext() = default;

		[[nodiscard]] virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual void BeginRender(const std::shared_ptr<Render::Camera>& camera) = 0;
		virtual void EndRender() = 0;

		virtual void DrawQuad(Vector3 position, Vector3 rotation, Vector3 scale, Colour colour) = 0;
		virtual void Draw(const std::shared_ptr<IMesh>& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour) = 0;

		virtual void ResizeViewport(const Vector2Int& size) = 0;

		/* This returns the currently selected Graphics API (OpenGL or DX11) */
		static std::shared_ptr<IRenderContext> Create();
	};
}//namespace Engine::Render