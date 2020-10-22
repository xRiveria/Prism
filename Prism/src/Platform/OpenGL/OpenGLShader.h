#pragma once
#include "Renderer/Shader.h"

namespace Prism
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSourceCode, const std::string& fragmentSourceCode);
		~OpenGLShader();

		virtual void BindShader() const override;
		virtual void UnbindShader() const override;

		void UploadUniformInt(const std::string& uniformName, const int& value);
		void UploadUniformFloat(const std::string& uniformName, const float& value);
		void UploadUniformFloat2(const std::string& uniformName, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& uniformName, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& uniformName, const glm::vec4& values);
		void UploadUniformMat3(const std::string& uniformName, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& uniformName, const glm::mat4& matrix);

	private:
		uint32_t m_ShaderID;
	};
}
