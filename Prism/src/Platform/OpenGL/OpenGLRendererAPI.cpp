#include "PrismPrecompiledHeader.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace Prism
{
	void OpenGLRendererAPI::InitializeRenderer()
	{
		PRISM_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t windowWidth, uint32_t windowHeight)
	{
		glViewport(x, y, windowWidth, windowHeight);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Reference<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetIndicesCount(), GL_UNSIGNED_INT, nullptr);
	}
}