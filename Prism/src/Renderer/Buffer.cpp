#include "PrismPrecompiledHeader.h"
#include "Buffer.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Prism
{
	VertexBuffer* VertexBuffer::CreateVertexBuffer(float* vertices, uint32_t size)
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
				return new OpenGLVertexBuffer(vertices, size);
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown Rendering API!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::CreateIndexBuffer(uint32_t* indices, uint32_t size)
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
				return new OpenGLIndexBuffer(indices, size);
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown Rendering API!");
		return nullptr;
	}
}