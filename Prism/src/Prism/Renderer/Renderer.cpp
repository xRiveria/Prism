#include "PrismPrecompiledHeader.h"
#include "Prism/Renderer/Renderer.h"
#include "Prism/Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Prism/Renderer/Renderer2D.h"

//This is the main renderer. We will spin off and create the 2D renderer when it is initialized.
namespace Prism
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::InitializeRenderer()
	{
		PRISM_PROFILE_FUNCTION();

		RenderCommand::InitializeRenderer();
		Renderer2D::Initialize2DRenderer();
	}

	void Renderer::OnWindowResize(uint32_t windowWidth, uint32_t windowHeight)
	{
		RenderCommand::SetViewport(0, 0, windowWidth, windowHeight);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	//We cast it to OpenGL Shader here for now as its the only render API we have.
	//By the time we are ready for DirectX or Vulkan, we will be ready for that.
	//Thus, we will do casting for now. This is temporary and the system will be rewritten by then.
	void Renderer::SubmitToRenderQueue(const Reference<Shader>& shader, const Reference<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->BindShader();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->viewProjectionMatrix); //Does this really have to be done per object?
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform); //Must be done per object. 

		vertexArray->BindVertexArray();

		RenderCommand::DrawIndexed(vertexArray);
	}
}