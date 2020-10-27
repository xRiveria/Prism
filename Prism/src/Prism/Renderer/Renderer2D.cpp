#include "PrismPrecompiledHeader.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Prism
{
	struct Renderer2DStorage
	{
		Reference<Prism::VertexArray> m_QuadVertexArray;
		Reference<Prism::Shader> m_TextureShader;
		Reference<Prism::Texture2D> m_WhiteTexture;
	};

	static Renderer2DStorage* s_Data; 

	void Renderer2D::Initialize2DRenderer()
	{
		PRISM_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		s_Data->m_QuadVertexArray = VertexArray::CreateVertexArray();

		float squareVertices[5 * 4]
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Reference<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::CreateVertexBuffer(squareVertices, sizeof(squareVertices)));
		
		squareVertexBuffer->SetBufferLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
			    { ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->m_QuadVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Reference<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::CreateIndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->m_QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_Data->m_WhiteTexture = Texture2D::CreateTexture(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->m_WhiteTexture->SetTextureData(&whiteTextureData, sizeof(whiteTextureData));

		s_Data->m_TextureShader = Shader::CreateShader("assets/shaders/Texture.glsl");
		s_Data->m_TextureShader->BindShader();
		s_Data->m_TextureShader->SetShaderInteger("u_Texture", 0);
	}

	void Renderer2D::Shutdown2DRenderer()
	{
		PRISM_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		PRISM_PROFILE_FUNCTION();

		s_Data->m_TextureShader->BindShader();
		s_Data->m_TextureShader->SetShaderMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		PRISM_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, const glm::vec4& quadColor)
	{
		DrawQuad({quadPosition.x, quadPosition.y, 0.0f}, quadSize, quadColor);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, const glm::vec4& quadColor)
	{
		PRISM_PROFILE_FUNCTION();

		s_Data->m_TextureShader->SetShaderFloat4("u_Color", quadColor);
		s_Data->m_WhiteTexture->BindTexture();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), quadPosition) * glm::scale(glm::mat4(1.0f), {quadSize.x, quadSize.y, 1.0f});
		s_Data->m_TextureShader->SetShaderMat4("u_Transform", transform);

		s_Data->m_QuadVertexArray->BindVertexArray();
		RenderCommand::DrawIndexed(s_Data->m_QuadVertexArray);
	}

	//======== Texture ========

	void Renderer2D::DrawQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, const Reference<Texture2D>& quadTexture)
	{
		DrawQuad({ quadPosition.x, quadPosition.y, 0.0f }, quadSize, quadTexture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, const Reference<Texture2D>& quadTexture)
	{
		PRISM_PROFILE_FUNCTION();

		s_Data->m_TextureShader->SetShaderFloat4("u_Color", glm::vec4(1.0f));
		quadTexture->BindTexture();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), quadPosition) * glm::scale(glm::mat4(1.0f), { quadSize.x, quadSize.y, 1.0f });
		s_Data->m_TextureShader->SetShaderMat4("u_Transform", transform);

		s_Data->m_QuadVertexArray->BindVertexArray();
		RenderCommand::DrawIndexed(s_Data->m_QuadVertexArray);
	}
}