#include "Engine.h"
#include "Render.h"

#include "IRenderContext.h"
#include "Resources/Camera.h"

namespace Engine
{
	bool Renderer::Init()
	{
		s_RenderContext = Render::IRenderContext::Create();
		SE_ASSERT(s_RenderContext != nullptr, "Render Context is Null");
		SE_ASSERT(s_RenderContext->Init(), "Render Context could not be initialized");

		s_Camera->Init();

		return true;
	}

	void Renderer::Destroy()
	{
		s_RenderContext->Destroy();
	}

	void Renderer::BeginRender()
	{
		s_RenderContext->BeginRender(s_Camera);
	}

	void Renderer::EndRender()
	{
		s_RenderContext->EndRender();
	}

	void Renderer::Draw(const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour)
	{
		s_RenderContext->DrawQuad(position, rotation, scale, colour);
	}

	void Renderer::DrawMesh(const std::shared_ptr<Render::IMesh>& mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale, Colour colour)
	{
		s_RenderContext->Draw(mesh, position, rotation, scale, colour);
	}

	void Renderer::SetViewportSize(const Vector2Int& size)
	{
		if (!s_RenderContext)
		{
			return;	
		}
		s_RenderContext->ResizeViewport(size);
	}

	Colour Renderer::GetClearColour()
	{
		return s_ClearColour;
	}

	void Renderer::SetClearColour(Colour colour)
	{
		s_ClearColour = colour;
	}
	void Renderer::SetCamera(const std::shared_ptr<Render::Camera>& camera)
	{
		s_Camera = camera;
	}
	std::shared_ptr<Render::Camera> Renderer::GetCamera()
	{
		return s_Camera;
	}
}//namespace Engine