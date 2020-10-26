#include "PrismPrecompiledHeader.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Prism
{
	Reference<Shader> Shader::CreateShader(const std::string& filePath)
	{
		switch (Renderer::GetCurrentRenderAPI())
		{
			case RendererAPI::RenderAPI::None:
			{
				PRISM_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported.");
				return nullptr;
			}

			case RendererAPI::RenderAPI::OpenGL:
			{
				return std::make_shared<OpenGLShader>(filePath);
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}

	Reference<Shader> Shader::CreateShader(const std::string& shaderName, std::string& vertexSourceCode, const std::string& fragmentSourceCode)
	{
		switch (Renderer::GetCurrentRenderAPI())
		{
			case RendererAPI::RenderAPI::None:
			{
				PRISM_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported.");
				return nullptr;
			}

			case RendererAPI::RenderAPI::OpenGL:
			{
				return std::make_shared<OpenGLShader>(shaderName, vertexSourceCode, fragmentSourceCode);
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}

	void ShaderLibrary::AddShader(const std::string& shaderName, const Reference<Shader>& shader)
	{
		PRISM_ENGINE_ASSERT(!DoesShaderExist(shaderName), "Shader already exists!");
		m_Shaders[shaderName] = shader;
	}

	void ShaderLibrary::AddShader(const Reference<Shader>& shader)
	{
		const std::string& shaderName = shader->GetShaderName();
		AddShader(shaderName, shader);
	}

	Reference<Shader> ShaderLibrary::LoadShader(const std::string& filePath)
	{
		Reference<Shader> shader = Shader::CreateShader(filePath);
		AddShader(shader);
		return shader;
	}

	Reference<Shader> ShaderLibrary::LoadShader(const std::string& shaderName, const std::string& filePath)
	{
		Reference<Shader> shader = Shader::CreateShader(filePath);
		AddShader(shaderName, shader);
		return shader;
	}

	Reference<Shader> ShaderLibrary::GetShader(const std::string& shaderName)
	{
		PRISM_ENGINE_ASSERT(DoesShaderExist(shaderName), "Shader not found!");
		return m_Shaders[shaderName];
	}

	bool ShaderLibrary::DoesShaderExist(const std::string& shaderName)
	{
		return m_Shaders.find(shaderName) != m_Shaders.end();
	}
}
