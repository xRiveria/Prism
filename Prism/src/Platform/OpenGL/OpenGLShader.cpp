#include "PrismPrecompiledHeader.h"
#include "OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include "glad/glad.h"

namespace Prism
{
	static GLenum ShaderTypeFromString(const std::string& typeString)
	{
		if (typeString == "VertexShader")
		{
			return GL_VERTEX_SHADER;
		}

		if (typeString == "FragmentShader" || typeString == "PixelShader")
		{
			return GL_FRAGMENT_SHADER;
		}

		PRISM_ENGINE_ASSERT(false, "Unknown Shader Type.");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		PRISM_PROFILE_FUNCTION();

		std::string sourceFile = ReadFile(filePath);
		auto shaderSources = PreProcessFile(sourceFile);
		CompileShader(shaderSources);


		//Extract name from file path. For C++17 and above, we can use std::filesystem::path::stem.
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;

		m_ShaderName = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& shaderName, const std::string& vertexSourceCode, const std::string& fragmentSourceCode) : m_ShaderName(shaderName)
	{
		PRISM_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSourceCode;
		sources[GL_FRAGMENT_SHADER] = fragmentSourceCode;
		CompileShader(sources);
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		PRISM_PROFILE_FUNCTION();

		std::string fileResult;
		std::ifstream inputFile(filePath, std::ios::in | std::ios::binary); //We read it as a binary because we don't want to do any form of processing to it. We don't C++ to intepret it as a string or whatsoever.
		if (inputFile)
		{
			inputFile.seekg(0, std::ios::end); //Moves the file pointer to the end of the file.
			fileResult.resize(inputFile.tellg()); //Tells us where the file pointer is. As its at the end, its the size of the file.
			inputFile.seekg(0, std::ios::beg); //Brings the file pointer back to the top of the file.
			inputFile.read(&fileResult[0], fileResult.size()); //Read from the beginning of the file and the size to read is the size of the file.
			inputFile.close();
		}
		else
		{
			PRISM_ENGINE_ERROR("Could not open GLSL file at {0}", filePath);
		}
		return fileResult;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcessFile(const std::string& sourceFile)
	{
		PRISM_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;
		
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t position = sourceFile.find(typeToken, 0); //Find the position of typeToken in the code.
		while (position != std::string::npos) //While we're finding it...
		{
			size_t eol = sourceFile.find_first_of("\r\n", position); //Find the first instance of a new line or a carriage return.
			PRISM_ENGINE_ASSERT(eol != std::string::npos, "Syntax Error."); //If the lines don't exist, error.
			size_t begin = position + typeTokenLength + 1; //The position of that first typeToken. We +1 due to the space before "FragmentShader"/"VertexShader" = #typeToken FragmentShader. 
			std::string type = sourceFile.substr(begin, eol - begin); //Extract that type. Take a substring from beginning to the end of the line minus the typeToken + 1 = just the type name.
			PRISM_ENGINE_ASSERT(ShaderTypeFromString(type), "Invalid Shader Type Specified."); //If its not equal to any of these, invalid shader type.
		
			size_t nextLinePosition = sourceFile.find_first_not_of("\r\n", eol); //Find next line from the end of the line above.
			position = sourceFile.find(typeToken, nextLinePosition); //Find next typeToken.
			shaderSources[ShaderTypeFromString(type)] = sourceFile.substr(nextLinePosition, position - (nextLinePosition == std::string::npos ? sourceFile.size() - 1 : nextLinePosition));
		}

		return shaderSources;
	}

	void OpenGLShader::CompileShader(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		PRISM_PROFILE_FUNCTION();

		GLuint shaderProgramID = glCreateProgram();
		PRISM_ENGINE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now.");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		for (auto& keyValue : shaderSources)
		{
			GLenum shaderType = keyValue.first;
			const std::string& sourceFile = keyValue.second;

			GLuint shader = glCreateShader(shaderType);
			const char* vertexSourceChar = sourceFile.c_str();
			glShaderSource(shader, 1, &vertexSourceChar, 0);
			glCompileShader(shader);

			int isShaderCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isShaderCompiled);
			if (isShaderCompiled == GL_FALSE)
			{
				int maxDebugLength = 64;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxDebugLength);
				//The max length includes the null character.
				std::vector<char> infoLog(maxDebugLength);
				glGetShaderInfoLog(shader, maxDebugLength, &maxDebugLength, &infoLog[0]);
				glDeleteShader(shader);

				PRISM_ENGINE_ERROR("{0}", infoLog.data());
				PRISM_ENGINE_ASSERT(0, "Shader Compilation Failure!");
				break;
			}

			glAttachShader(shaderProgramID, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(shaderProgramID);

		int isShaderLinked;
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &isShaderLinked);
		if (isShaderLinked == GL_FALSE)
		{
			int maxDebugLength = 64;
			glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxDebugLength);
			//The max length includes the null character.
			std::vector<char> infoLog(maxDebugLength);
			glGetProgramInfoLog(shaderProgramID, maxDebugLength, &maxDebugLength, &infoLog[0]);

			glDeleteProgram(shaderProgramID);

			PRISM_ENGINE_ERROR("{0}", infoLog.data());
			PRISM_ENGINE_ASSERT(0, "Shader Linking Failure!");
			return;
		}

		//Always detach shaders after a successful link.
		for (auto shaderID : glShaderIDs)
		{
			glDetachShader(shaderProgramID, shaderID);
		}

		m_ShaderID = shaderProgramID;
	}

	OpenGLShader::~OpenGLShader()
	{
		PRISM_PROFILE_FUNCTION();

		glDeleteProgram(m_ShaderID);
	}

	void OpenGLShader::BindShader() const
	{
		PRISM_PROFILE_FUNCTION();

		glUseProgram(m_ShaderID);
	}

	void OpenGLShader::UnbindShader() const
	{
		PRISM_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetShaderInteger(const std::string& name, int value)
	{
		PRISM_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetShaderFloat(const std::string& name, int value)
	{
		PRISM_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetShaderFloat3(const std::string& name, const glm::vec3& value)
	{
		PRISM_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetShaderFloat4(const std::string& name, const glm::vec4& value)
	{
		PRISM_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetShaderMat4(const std::string& name, const glm::mat4& value)
	{
		PRISM_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
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

