#include "PrismPrecompiledHeader.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Prism
{
	struct Renderer2DStorage
	{
		Reference<Prism::Shader> m_FlatColorShader;
		Reference<Prism::VertexArray> m_QuadVertexArray;
	};

	static Renderer2DStorage* s_Data; 

	void Renderer2D::Initialize2DRenderer()
	{
		s_Data = new Renderer2DStorage();

		s_Data->m_QuadVertexArray = VertexArray::CreateVertexArray();

		float squareVertices[5 * 4]
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Reference<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::CreateVertexBuffer(squareVertices, sizeof(squareVertices)));
		
		squareVertexBuffer->SetBufferLayout(
			{
				{ ShaderDataType::Float3, "a_Position" }
			});
		s_Data->m_QuadVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Reference<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::CreateIndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->m_QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_Data->m_FlatColorShader = Shader::CreateShader("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown2DRenderer()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_FlatColorShader)->BindShader();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, const glm::vec4& quadColor)
	{
		DrawQuad({quadPosition.x, quadPosition.y, 0.0f}, quadSize, quadColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, const glm::vec4& quadColor)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_FlatColorShader)->BindShader();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->m_FlatColorShader)->UploadUniformFloat4("u_Color", quadColor);

		s_Data->m_QuadVertexArray->BindVertexArray();
		RenderCommand::DrawIndexed(s_Data->m_QuadVertexArray);
	}
}