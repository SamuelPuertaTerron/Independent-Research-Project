#pragma once
#include "Engine/Render/IShader.h"
#include "Engine/Render/Resources/IMesh.h"
#include "Engine/Render/Vertext.h"

#include "../../../../../ThirdParty/glm/glm/glm.hpp"
#include "../../../../../ThirdParty/glm/glm/gtc/matrix_transform.hpp"
#include "../../../../../ThirdParty/glm/glm/gtc/type_ptr.hpp"

namespace Engine::Render
{
	class IShader;
}//namespace Engine::Render

namespace Engine::Render::OpenGL
{
	struct TransformBuffer
	{
		Vector3 Position;
		Vector3 Rotation;
		Vector3 Scale;
	};

	class OpenGLMesh : public IMesh
	{
	public:
		OpenGLMesh() = default;
		virtual ~OpenGLMesh() override = default;

		virtual bool Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices = {}) override;
		virtual void Destroy() override;

		virtual void Render() override;

		void SetTransformBuffer(const TransformBuffer& buffer, const glm::mat4& projection, const glm::mat4& view);

		void BindShader() const
		{
			m_Shader->Bind();
		}

	private:
		unsigned int m_VAO, m_VBO, m_EBO;
		std::shared_ptr<IShader> m_Shader;
		TransformBuffer m_TransformBuffer;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
	};
}//namespace Engine::Render::OpenGL