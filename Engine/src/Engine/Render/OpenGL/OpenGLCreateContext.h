/* Creates the openGL context for the Windows window*/
#pragma once

#include <Windows.h>

namespace Engine::OpenGL
{
	class OpenGLCreateContext
	{
	public:
		/**
		 * @brief Creates an OpenGL Context
		 * @param dc The Device Context
		 * @param glContext The OpenGL Context for this window
		 * @param major The major version (Default 4)
		 * @param minor The minor version (default 6)
		 */
		static void CreateContext(const HDC& dc, HGLRC& glContext, int major = 4, int minor = 6); 
	};
}//namespace Engine::OpenGL