#pragma once
#include "Prism/Renderer/RendererAPI.h"

namespace Prism
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void InitializeRenderer() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t windowWidth, uint32_t windowHeight) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawGraphicsInformation() override;

		virtual void DrawIndexed(const Reference<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
	};
}