#include "Engine.h"
#include "OpenGLMesh.h"

#include "OpenGLShader.h"
#include "../../../../../ThirdParty/glad/include/glad/glad.h"
#include "../../../../../ThirdParty/glm/glm/glm.hpp"
#include "../../../../../ThirdParty/glm/glm/gtc/matrix_transform.hpp"
#include "../../../../../ThirdParty/glm/glm/gtc/type_ptr.hpp"
#include "Engine/Render/IShader.h"

namespace Engine::Render::OpenGL
{
	bool OpenGLMesh::Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		m_Vertices = vertices;
		m_Indices = indices;

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		if (!indices.empty())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
		}

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glEnableVertexAttribArray(0);

		// Normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);

		// Color attribute
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colour));

		glEnableVertexAttribArray(2);
		glBindVertexArray(0);

		m_Shader = IShader::Create(EShaderType::None); //The shader type does not matter for OpenGL shaders

		m_Shader->Compile(nullptr, "Resources/default.glsl");

		return true;
	}

	void OpenGLMesh::Destroy()
	{
	}

	void OpenGLMesh::Render()
	{
		m_Shader->Bind();

		// Compute model matrix
		Vector3 position = m_TransformBuffer.Position;
		Vector3 rotation = m_TransformBuffer.Rotation;
		Vector3 scale = m_TransformBuffer.Scale;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.X, position.Y, position.Z));
		model = glm::rotate(model, glm::radians(rotation.X), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.Y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.Z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale.X, scale.Y, scale.Z));

		// Set uniforms
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderID(), "u_Model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderID(), "u_View"), 1, GL_FALSE, glm::value_ptr(m_ViewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderID(), "u_Projection"), 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));

		glBindVertexArray(m_VAO);
		if (!m_Indices.empty()) {
			glDrawElements(GL_TRIANGLES, static_cast<int>(m_Indices.size()), GL_UNSIGNED_INT, nullptr);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(m_Vertices.size()));
		}
		glBindVertexArray(0);
	}
	void OpenGLMesh::SetTransformBuffer(const TransformBuffer& buffer, const glm::mat4& projection, const glm::mat4& view)
	{
		m_TransformBuffer = buffer;
		m_ProjectionMatrix = projection;
		m_ViewMatrix = view;
	}
}//namespace Engine::Render::OpenGL