#include "PrismPrecompiledHeader.h"
#include "Framebuffer.h"
#include "Prism/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Prism
{
	Prism::Reference<Prism::Framebuffer> Framebuffer::CreateFramebuffer(const FramebufferSpecification& bufferSpecifications)
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
				return CreateReference<OpenGLFramebuffer>(bufferSpecifications);
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}
}