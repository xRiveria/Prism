#pragma once
#include <string>
#include "glm/glm.hpp"

namespace Prism
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSourceCode, const std::string& fragmentSourceCode);
		~Shader();

		void BindShader() const;
		void UnbindShader() const; 

		void UploadUniformMat4(const std::string& uniformName, const glm::mat4& matrix);
		void UploadUniformFloat4(const std::string& uniformName, const glm::vec4& values);

	private:
		uint32_t m_ShaderID;
	};
}
