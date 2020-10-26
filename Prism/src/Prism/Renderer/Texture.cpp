#include "PrismPrecompiledHeader.h"
#include "Texture.h"
#include "Prism/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Prism
{
	Reference<Texture2D> Texture2D::CreateTexture(const std::string& filePath)
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
				return std::make_shared<OpenGLTexture2D>(filePath);
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}
}