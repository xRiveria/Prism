#include "PrismPrecompiledHeader.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Prism
{
	Shader* Shader::CreateShader(const std::string& vertexSourceCode, const std::string& fragmentSourceCode)
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
				return new OpenGLShader(vertexSourceCode, fragmentSourceCode);
			}

			PRISM_ENGINE_ASSERT(false, "Unknown RendererAPI.");
			return nullptr;
		}
	}
}
