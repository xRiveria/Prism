#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Prism/Renderer/Renderer.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_SquareVertexArray = Prism::VertexArray::CreateVertexArray();

	float squareVertices[5 * 4]
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Prism::Reference<Prism::VertexBuffer> squareVertexBuffer;
	squareVertexBuffer.reset(Prism::VertexBuffer::CreateVertexBuffer(squareVertices, sizeof(squareVertices)));
	squareVertexBuffer->SetBufferLayout(
		{
			{ Prism::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Prism::Reference<Prism::IndexBuffer> squareIndexBuffer;
	squareIndexBuffer.reset(Prism::IndexBuffer::CreateIndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

	m_FlatColorShader = Prism::Shader::CreateShader("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Prism::Timestep timeStep)
{
	//Update
	m_CameraController.OnUpdate(timeStep);

	//Render
	Prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Prism::RenderCommand::Clear();

	Prism::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Prism::OpenGLShader>(m_FlatColorShader)->BindShader();
	std::dynamic_pointer_cast<Prism::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Prism::Renderer::SubmitToRenderQueue(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	Prism::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Prism::Event& event)
{
	m_CameraController.OnEvent(event);
}
