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
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Reference<VertexArray>& vertexArray) = 0;
		inline static RenderAPI GetCurrentRenderAPI() { return s_GraphicsAPI; }

	private:
		static RenderAPI s_GraphicsAPI;
	};
}