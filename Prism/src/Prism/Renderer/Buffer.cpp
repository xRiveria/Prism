#include "PrismPrecompiledHeader.h"
#include "Buffer.h"
#include "Prism/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Prism
{
	Reference<VertexBuffer> VertexBuffer::CreateVertexBuffer(uint32_t size)
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
				return CreateReference<OpenGLVertexBuffer>(size);
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown Rendering API!");
		return nullptr;
	}

	Reference<VertexBuffer> VertexBuffer::CreateVertexBuffer(float* vertices, uint32_t size)
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
				return CreateReference<OpenGLVertexBuffer>(vertices, size);
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown Rendering API!");
		return nullptr;
	}

	Reference<IndexBuffer> IndexBuffer::CreateIndexBuffer(uint32_t* indices, uint32_t size)
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
				return CreateReference<OpenGLIndexBuffer>(indices, size);
			}
		}

		PRISM_ENGINE_ASSERT(false, "Unknown Rendering API!");
		return nullptr;
	}
}