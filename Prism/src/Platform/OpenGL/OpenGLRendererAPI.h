#pragma once
#include "Renderer/RendererAPI.h"

namespace Prism
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void InitializeRenderer() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Reference<VertexArray>& vertexArray) override;
	};
}