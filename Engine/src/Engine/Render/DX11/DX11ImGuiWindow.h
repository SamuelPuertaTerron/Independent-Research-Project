/* The DX11 ImGui window creation */
#pragma once
#include "Engine/Application/IImGuiWindow.h"

namespace Engine::Render::DX11
{
	class DX11ImGuiWindow : public IImGuiWindow
	{
	public:
		DX11ImGuiWindow() = default;
		virtual ~DX11ImGuiWindow() override = default;

		virtual bool Init() override;
		virtual void Destroy() override;

		virtual void BeginRender() override;
		virtual void EndRender() override;
	};
}//namespace Engine::Render::DX11