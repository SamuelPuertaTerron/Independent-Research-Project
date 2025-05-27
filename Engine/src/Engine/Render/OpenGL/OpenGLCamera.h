#pragma once
#include "Engine/Render/Resources/ICameraContext.h"

#include "../../../../../ThirdParty/glm/glm/glm.hpp"
#include "../../../../../ThirdParty/glm/glm/gtc/matrix_transform.hpp"
#include "../../../../../ThirdParty/glm/glm/gtc/type_ptr.hpp"

namespace Engine::Render::OpenGL
{
	class OpenGLCamera : public ICameraContext
	{
	public:
		OpenGLCamera() = default;
		virtual ~OpenGLCamera() override = default;

		virtual void UpdateTransform(const Vector3& position, const Vector3& rotation) override;

		glm::mat4 GetProjection() const { return m_ProjectMatrix; }
		glm::mat4 GetView() const { return m_ViewMatrix; }
	private:
		glm::mat4 m_ProjectMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	};

}//namespace Engine::Render::OpenGL