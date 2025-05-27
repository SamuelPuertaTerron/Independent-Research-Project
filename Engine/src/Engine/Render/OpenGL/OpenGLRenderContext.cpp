#include "Engine.h"
#include "OpenGLRenderContext.h"

#include "OpenGLCamera.h"
#include "OpenGLMesh.h"
#include "../../../../../ThirdParty/glad/include/glad/glad.h"
#include "Engine/Application/Window.h"

#include "Engine/Render/Resources/Camera.h"
#include "Engine/Render/Resources/IMesh.h"

namespace Engine::Render::OpenGL
{
	bool OpenGLRenderContext::Init()
	{
		if (!gladLoadGLLoader((GLADloadproc)GetProcAddress))
		{
			Logger::Log("Failed to load OpenGL functions", ELogLevel::Error);
			return false;
		}

		glFrontFace(GL_CW);
		glEnable(GL_BLEND); // Enabled Transparency
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ResizeViewport({ 1270, 720 });

		Logger::Log("Created OpenGL Graphics Backend", ELogLevel::Info);
		return true;
	}

	void OpenGLRenderContext::Destroy()
	{
		Logger::Log("Destroyed OpenGL Graphics Backend", ELogLevel::Info);
	}

	void OpenGLRenderContext::BeginRender(const std::shared_ptr<Render::Camera>& camera)
	{
		const auto& glCamera = dynamic_cast<OpenGLCamera*>(camera->GetContext().get());
		camera->UpdateTransform();

		m_RenderData.CameraProjectionMatrix = glCamera->GetProjection();
		m_RenderData.CameraViewMatrix = glCamera->GetView();

		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderContext::EndRender()
	{
		static auto window = Application::GetInstance().GetWindow();
		window->SwapBuffers();
	}

	void OpenGLRenderContext::Draw(const std::shared_ptr<Render::IMesh>& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour)
	{
		OpenGLMesh* glMesh = dynamic_cast<OpenGLMesh*>(mesh.get());
		glMesh->BindShader();

		TransformBuffer buffer;
		buffer.Position = position;
		buffer.Rotation = rotation;
		buffer.Scale = scale;

		glMesh->SetTransformBuffer(buffer, m_RenderData.CameraProjectionMatrix, m_RenderData.CameraViewMatrix);

		mesh->Render();
	}

	void OpenGLRenderContext::DrawQuad(Vector3 position, Vector3 rotation, Vector3 scale, Colour colour)
	{

	}

	void OpenGLRenderContext::ResizeViewport(const Vector2Int& size)
	{
		glViewport(0, 0, size.X, size.Y);
	}
}//namespace Engine::Render::OpenGL
