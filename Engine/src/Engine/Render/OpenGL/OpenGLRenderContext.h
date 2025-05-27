/* The OpenGL render context abstract layer */
#pragma once
#include "Engine/Render/IRenderContext.h"

#include "../../../../../ThirdParty/glm/glm/glm.hpp"
#include "../../../../../ThirdParty/glm/glm/gtc/matrix_transform.hpp"
#include "../../../../../ThirdParty/glm/glm/gtc/type_ptr.hpp"

namespace Engine::Render::OpenGL
{
	struct RenderData
	{
		glm::mat4 CameraProjectionMatrix;
		glm::mat4 CameraViewMatrix;
	};

	class OpenGLRenderContext : public Render::IRenderContext
	{
	public:
		OpenGLRenderContext() = default;
		virtual ~OpenGLRenderContext() override = default;

		virtual bool Init() override;
		virtual void Destroy() override;

		virtual void BeginRender(const std::shared_ptr<Render::Camera>& camera) override;
		virtual void EndRender() override;

		virtual void Draw(const std::shared_ptr<Render::IMesh>& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour) override;
		virtual void DrawQuad(Vector3 position, Vector3 rotation, Vector3 scale, Colour colour) override;

		virtual void ResizeViewport(const Vector2Int& size) override;

	private:
		RenderData m_RenderData;
	};
}//namespace Engine::Render::OpenGL
