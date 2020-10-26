#include "PrismPrecompiledHeader.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Prism
{
	Reference<VertexArray> VertexArray::CreateVertexArray()
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
				return std::make_shared<OpenGLVertexArray>();
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}
}