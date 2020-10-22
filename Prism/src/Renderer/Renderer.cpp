#include "PrismPrecompiledHeader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"

namespace Prism
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::SubmitToRenderQueue(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->BindShader();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->viewProjectionMatrix);
		vertexArray->BindVertexArray();

		RenderCommand::DrawIndexed(vertexArray);
	}
}