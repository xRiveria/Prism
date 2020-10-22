#include "PrismPrecompiledHeader.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Prism
{
	OpenGLShader::OpenGLShader(const std::string& vertexSourceCode, const std::string& fragmentSourceCode)
	{
		//Vertex Shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexSource = vertexSourceCode.c_str();
		glShaderSource(vertexShader, 1, &vertexSource, 0);
		glCompileShader(vertexShader);

		int isShaderCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);
		if (isShaderCompiled == GL_FALSE)
		{
			int maxDebugLength = 64;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxDebugLength);
			//The max length includes the null character.
			std::vector<char> infoLog(maxDebugLength);
			glGetShaderInfoLog(vertexShader, maxDebugLength, &maxDebugLength, &infoLog[0]);
			glDeleteShader(vertexShader);

			PRISM_ENGINE_ERROR("{0}", infoLog.data());
			PRISM_ENGINE_ASSERT(0, "Vertex Shader Compilation Failure!");
			return;
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragmentSource = fragmentSourceCode.c_str();
		glShaderSource(fragmentShader, 1, &fragmentSource, 0);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiled);
		if (isShaderCompiled == GL_FALSE)
		{
			int maxDebugLength = 64;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxDebugLength);
			//The max length includes the null character.
			std::vector<char> infoLog(maxDebugLength);
			glGetShaderInfoLog(fragmentShader, maxDebugLength, &maxDebugLength, &infoLog[0]);
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader); //Don't leak shaders.

			PRISM_ENGINE_ERROR("{0}", infoLog.data());
			PRISM_ENGINE_ASSERT(0, "Fragment Shader Compilation Failure!");
			return;
		}

		m_ShaderID = glCreateProgram();
		glAttachShader(m_ShaderID, vertexShader);
		glAttachShader(m_ShaderID, fragmentShader);
		glLinkProgram(m_ShaderID);

		int isShaderLinked;
		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &isShaderLinked);
		if (isShaderLinked == GL_FALSE)
		{
			int maxDebugLength = 64;
			glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &maxDebugLength);
			//The max length includes the null character.
			std::vector<char> infoLog(maxDebugLength);
			glGetProgramInfoLog(m_ShaderID, maxDebugLength, &maxDebugLength, &infoLog[0]);

			glDeleteProgram(m_ShaderID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			PRISM_ENGINE_ERROR("{0}", infoLog.data());
			PRISM_ENGINE_ASSERT(0, "Shader Linking Failure!");
			return;
		}
		//Always detach shaders after a successful link.
		glDetachShader(m_ShaderID, vertexShader);
		glDetachShader(m_ShaderID, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ShaderID);
	}

	void OpenGLShader::BindShader() const
	{
		glUseProgram(m_ShaderID);
	}

	void OpenGLShader::UnbindShader() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& uniformName, const int& value)
	{
		GLint location = glGetUniformLocation(m_ShaderID, uniformName.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& uniformName, const float& value)
	{
		GLint location = glGetUniformLocation(m_ShaderID, uniformName.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& uniformName, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_ShaderID, uniformName.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& uniformName, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_ShaderID, uniformName.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& uniformName, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_ShaderID, uniformName.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}
	
	void OpenGLShader::UploadUniformMat3(const std::string& uniformName, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_ShaderID, uniformName.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& uniformName, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_ShaderID, uniformName.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}