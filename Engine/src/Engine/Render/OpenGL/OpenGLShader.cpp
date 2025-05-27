#include "Engine.h"
#include "OpenGLShader.h"

#include "../../../../../ThirdParty/glad/include/glad/glad.h"

namespace Engine::Render::OpenGL
{
	void OpenGLShader::Compile(IRenderContext* context, const fs::path& path)
	{
		if (!fs::exists(path)) 
		{
			Logger::Log("Shader file not found: " + path.string(), ELogLevel::Error);
			return;
		}

		std::ifstream in(path);
		std::stringstream buffer;
		buffer << in.rdbuf();
		const std::string text = buffer.str();

		std::string vertexShader;
		std::string fragmentShader;
		std::string tessellationControlShader;
		std::string tessellationEvaluationShader;
		std::string geometryShader;

		ExtractShaders(text, vertexShader, fragmentShader, tessellationControlShader, tessellationEvaluationShader, geometryShader);

		GLint success;
		GLchar infoLog[512];

		const char* vertexShaderCode = vertexShader.c_str();
		const char* fragmentShaderCode = fragmentShader.c_str();
		const char* tessellationControlShaderCode = tessellationControlShader.empty() ? nullptr : tessellationControlShader.c_str();
		const char* tessellationEvaluationShaderCode = tessellationEvaluationShader.empty() ? nullptr : tessellationEvaluationShader.c_str();
		const char* geometryShaderCode = geometryShader.empty() ? nullptr : geometryShader.c_str();

		const int vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::cout << "Error in Vertex Shader Compilation: " << path.filename() << " " << infoLog << "\n";
			throw;
		}

		const int fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
			std::cout << "Error in Fragment Shader Compilation: " << infoLog << "\n";
			throw;
		}

		int tessControl = 0, tessEvaluation = 0;
		if (tessellationControlShaderCode && tessellationEvaluationShaderCode)
		{
			tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(tessControl, 1, &tessellationControlShaderCode, nullptr);
			glCompileShader(tessControl);

			glGetShaderiv(tessControl, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(tessControl, 512, nullptr, infoLog);
				std::cout << "Error in tessControl Shader Compilation: " << infoLog << "\n";
				throw;
			}

			tessEvaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(tessEvaluation, 1, &tessellationEvaluationShaderCode, nullptr);
			glCompileShader(tessEvaluation);

			glGetShaderiv(tessEvaluation, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(tessEvaluation, 512, nullptr, infoLog);
				std::cout << "Error in tessEvaluation Shader Compilation: " << infoLog << "\n";
				throw;
			}
		}

		int geometry = 0;
		if (geometryShaderCode)
		{
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &geometryShaderCode, nullptr);
			glCompileShader(geometry);

			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
				std::cout << "Error in Geometry Shader Compilation: " << infoLog << "\n";
				throw;
			}
		}

		m_ShaderID = glCreateProgram();

		glAttachShader(m_ShaderID, vertex);
		glAttachShader(m_ShaderID, fragment);

		if (tessellationControlShaderCode) glAttachShader(m_ShaderID, tessControl);
		if (tessellationEvaluationShaderCode) glAttachShader(m_ShaderID, tessEvaluation);

		if (geometryShaderCode) glAttachShader(m_ShaderID, geometry);

		glLinkProgram(m_ShaderID);

		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_ShaderID, 512, nullptr, infoLog);
			std::cout << "Error in Linking Shader Compilation: " << infoLog << "\n";
			throw;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		if (tessellationControlShaderCode) glDeleteShader(tessControl);
		if (tessellationEvaluationShaderCode) glDeleteShader(tessEvaluation);

		if (geometryShaderCode) glDeleteShader(geometry);
	}

	void OpenGLShader::Destroy()
	{
		
	}
	void OpenGLShader::Bind()
	{
		glUseProgram(m_ShaderID);
	}
	void OpenGLShader::ExtractShaders(const std::string& fileContent, std::string& vertexShader, std::string& fragmentShader, std::string& tessControl, std::string& tessEval, std::string& geometryShader)
	{
		const std::string vertexStart = "// --- Vertex Shader Start ---";
		const std::string vertexEnd = "// --- Vertex Shader End ---";

		const std::string fragmentStart = "// --- Fragment Shader Start ---";
		const std::string fragmentEnd = "// --- Fragment Shader End ---";

		const size_t vertexStartPos = fileContent.find(vertexStart);
		const size_t vertexEndPos = fileContent.find(vertexEnd);
		const size_t fragmentStartPos = fileContent.find(fragmentStart);
		const size_t fragmentEndPos = fileContent.find(fragmentEnd);

		if (vertexStartPos != std::string::npos && vertexEndPos != std::string::npos) {
			vertexShader = fileContent.substr(vertexStartPos + vertexStart.size(), vertexEndPos - vertexStartPos - vertexStart.size());
		}

		if (fragmentStartPos != std::string::npos && fragmentEndPos != std::string::npos) {
			fragmentShader = fileContent.substr(fragmentStartPos + fragmentStart.size(), fragmentEndPos - fragmentStartPos - fragmentStart.size());
		}
	}
}//namespace Engine::Render::OpenGL