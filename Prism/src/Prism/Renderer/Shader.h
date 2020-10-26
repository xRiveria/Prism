#pragma once
#include <string>
#include <unordered_map>

namespace Prism
{
	class Shader
	{
	public:
		~Shader() = default;

		virtual void BindShader() const = 0;
		virtual void UnbindShader() const = 0;

		virtual const std::string& GetShaderName() const = 0;

		static Reference<Shader> CreateShader(const std::string& filePath);
		static Reference<Shader> CreateShader(const std::string& shaderName, std::string& vertexSourceCode, const std::string& fragmentSourceCode);
	};

	class ShaderLibrary //Stores a reference to each created Shader in our application. When we do have a standard 2D Shader, Blur Shader, Postprocessing Shader, PBR Shader etc, we will use a shader library to load these shaders in for use by the entire codebase.
	{
	public:
		void AddShader(const std::string& shaderName, const Reference<Shader>& shader);
		void AddShader(const Reference<Shader>& shader);
		Reference<Shader> LoadShader(const std::string& filePath); //Loads in a shader file from a filePath. Texture.glsl will be stored as Texture.
		Reference<Shader> LoadShader(const std::string& shaderName, const std::string& filePath); //Custom name.

		Reference<Shader> GetShader(const std::string& shaderName);
		bool DoesShaderExist(const std::string& shaderName);

	private:
		std::unordered_map<std::string, Reference<Shader>> m_Shaders;
	};
}
