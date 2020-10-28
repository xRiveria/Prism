#pragma once
#include "glm/glm.hpp"
#include "VertexArray.h"

namespace Prism
{
	class RendererAPI
	{
	public:
		enum class RenderAPI
		{
			None = 0,
			OpenGL = 1,
			DirectX = 2,
			Vulkan = 3
		};

	public:
		virtual void InitializeRenderer() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t windowWidth, uint32_t windowHeight) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Reference<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		inline static RenderAPI GetCurrentRenderAPI() { return s_GraphicsAPI; }

	private:
		static RenderAPI s_GraphicsAPI;
	};
}