#pragma once
#include "Engine/Application/IImGuiWindow.h"

namespace Engine::Render::OpenGL
{
	class OpenGLImGuiWindow : public IImGuiWindow
	{
	public:
		OpenGLImGuiWindow() = default;
		virtual ~OpenGLImGuiWindow() override = default;

		virtual bool Init() override;
		virtual void Destroy() override;

		virtual void BeginRender() override;
		virtual void EndRender() override;
	};
}//namespace Engine::Render::OpenGL
