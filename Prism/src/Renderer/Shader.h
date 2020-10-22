#pragma once
#include <string>
#include "glm/glm.hpp"

namespace Prism
{
	class Shader
	{
	public:
		~Shader() = default;

		virtual void BindShader() const = 0;
		virtual void UnbindShader() const = 0;

		static Shader* CreateShader(const std::string& vertexSourceCode, const std::string& fragmentSourceCode);
	};
}
