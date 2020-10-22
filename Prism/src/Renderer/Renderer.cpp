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

	void Renderer::SubmitToRenderQueue(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->BindShader();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->viewProjectionMatrix); //Does this really have to be done per object?
		shader->UploadUniformMat4("u_Transform", transform); //Must be done per object. 

		vertexArray->BindVertexArray();

		RenderCommand::DrawIndexed(vertexArray);
	}
}