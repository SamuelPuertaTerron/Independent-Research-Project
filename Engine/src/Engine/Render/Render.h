/* The main static class for rendering */
#pragma once

namespace Engine
{
	namespace Render
	{
		class IRenderContext;
		class IMesh;
		class Camera;
	}//namespace Render

	class Renderer
	{
	public:
		[[nodiscard]] static bool Init();
		static void Destroy();

		static void BeginRender();
		static void EndRender();

		static void Draw(const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour);
		static void DrawMesh(const std::shared_ptr<Render::IMesh>& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour);

		static void SetViewportSize(const Vector2Int& size);

		[[nodiscard]] static Colour GetClearColour();
		static void SetClearColour(Colour colour);

		static std::shared_ptr<Render::IRenderContext> GetRenderContext()
		{
			return s_RenderContext;
		}

		static void SetCamera(const std::shared_ptr<Render::Camera>& camera);
		static std::shared_ptr<Render::Camera> GetCamera();
	private:
		inline static std::shared_ptr<Render::IRenderContext> s_RenderContext;
		inline static Colour s_ClearColour;
		inline static std::shared_ptr<Render::Camera> s_Camera;
	};
}//namespace Engine