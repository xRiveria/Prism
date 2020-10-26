#pragma once
#include "Renderer/Shader.h"
#include "glm/glm.hpp"

typedef unsigned int GLenum;

namespace Prism
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& shaderName, const std::string& vertexSourceCode, const std::string& fragmentSourceCode);
		~OpenGLShader();

		virtual void BindShader() const override;
		virtual void UnbindShader() const override;

		virtual const std::string& GetShaderName() const override { return m_ShaderName; }

		void UploadUniformInt(const std::string& uniformName, const int& value);
		void UploadUniformFloat(const std::string& uniformName, const float& value);
		void UploadUniformFloat2(const std::string& uniformName, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& uniformName, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& uniformName, const glm::vec4& values);
		void UploadUniformMat3(const std::string& uniformName, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& uniformName, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcessFile(const std::string& sourceFile);
		void CompileShader(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_ShaderID;
		std::string m_ShaderName;
	};
}
