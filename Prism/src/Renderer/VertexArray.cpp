#include "PrismPrecompiledHeader.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Prism
{
	VertexArray* VertexArray::CreateVertexArray()
	{
		switch (Renderer::GetCurrentRenderAPI())
			{
			case RendererAPI::None:
			{
				PRISM_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported.");
				return nullptr;
			}

			case RendererAPI::OpenGL:
			{
				return new OpenGLVertexArray();
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown RendererAPI.");
		return nullptr;
	}
}