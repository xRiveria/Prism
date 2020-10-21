#include "PrismPrecompiledHeader.h"
#include "Renderer/Renderer.h"

namespace Prism
{
	void Renderer::BeginScene()
	{

	}

	void Renderer::EndScene()
	{

	}

	void Renderer::SubmitToRenderQueue(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->BindVertexArray();
		RenderCommand::DrawIndexed(vertexArray);
	}
}