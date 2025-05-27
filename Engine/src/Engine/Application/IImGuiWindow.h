/* Abstracts ImGui for the specific graphics API */
/* The abstraction are found within the OpenGL & DX11 folder under Render */

#pragma once

namespace Engine
{
	class IImGuiWindow
	{
	public:
		virtual ~IImGuiWindow() = default;

		virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;

		static std::shared_ptr<IImGuiWindow> Create();
	};
}//namespace Engine
