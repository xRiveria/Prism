#pragma once
#include <string>

namespace Prism
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSourceCode, const std::string& fragmentSourceCode);
		~Shader();

		void BindShader() const;
		void UnbindShader() const; 

	private:
		uint32_t m_ShaderID;
	};
}
