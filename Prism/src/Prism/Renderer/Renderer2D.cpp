#include "PrismPrecompiledHeader.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Prism
{
	struct QuadVertex
	{
		glm::vec3 m_QuadPosition;
		glm::vec4 m_QuadColor;
		glm::vec2 m_TexCoord;
		//To Do: Color, TexID, Masking
	};

	struct Renderer2DData
	{
		//Per Draw Call.
		const uint32_t m_MaxQuadsToDraw = 10000;
		const uint32_t m_MaxVertices = m_MaxQuadsToDraw * 4;
		const uint32_t m_MaxIndices = m_MaxQuadsToDraw * 6;

		Reference<Prism::VertexArray> m_QuadVertexArray;
		Reference<Prism::VertexBuffer> m_QuadVertexBuffer;
		Reference<Prism::Shader> m_TextureShader;
		Reference<Prism::Texture2D> m_WhiteTexture;

		uint32_t m_QuadIndexCount = 0; //Increments everytime a Quad is drawed.
		QuadVertex* m_QuadVertexBufferBase = nullptr; //To keep track of our max base amount of vertices.
		QuadVertex* m_QuadVertexBufferPointer = nullptr; //We need to create and push into the Vertex Buffer for new vertices.
	};

	static Renderer2DData s_Data; 



	void Renderer2D::Initialize2DRenderer()
	{
		PRISM_PROFILE_FUNCTION();

		s_Data.m_QuadVertexArray = VertexArray::CreateVertexArray();

		//======== Vertex Buffer (Vertices) ========

		/*float squareVertices[5 * 4]
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};*/

		s_Data.m_QuadVertexBuffer = VertexBuffer::CreateVertexBuffer(s_Data.m_MaxVertices * sizeof(QuadVertex));
		
		s_Data.m_QuadVertexBuffer->SetBufferLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
			    { ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data.m_QuadVertexArray->AddVertexBuffer(s_Data.m_QuadVertexBuffer);

		s_Data.m_QuadVertexBufferBase = new QuadVertex[s_Data.m_MaxVertices];
		//======== Index Buffer (Indices) ========

		uint32_t* quadIndices = new uint32_t[s_Data.m_MaxIndices];
		
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.m_MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Reference<IndexBuffer> quadIndexBuffer = IndexBuffer::CreateIndexBuffer(quadIndices, s_Data.m_MaxIndices);
		s_Data.m_QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
		delete[] quadIndices;

		//uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		
		s_Data.m_WhiteTexture = Texture2D::CreateTexture(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.m_WhiteTexture->SetTextureData(&whiteTextureData, sizeof(whiteTextureData));

		s_Data.m_TextureShader = Shader::CreateShader("assets/shaders/Texture.glsl");
		s_Data.m_TextureShader->BindShader();
		s_Data.m_TextureShader->SetShaderInteger("u_Texture", 0);
	}

	void Renderer2D::Shutdown2DRenderer()
	{
		PRISM_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		PRISM_PROFILE_FUNCTION();

		s_Data.m_TextureShader->BindShader();
		s_Data.m_TextureShader->SetShaderMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	
		s_Data.m_QuadIndexCount = 0; //Reset amount of indexes currently drawn.
		s_Data.m_QuadVertexBufferPointer = s_Data.m_QuadVertexBufferBase; //Set the pointer to the base. Now, we can freely increment the pointer without losing track of the base. 
	}

	void Renderer2D::EndScene()
	{
		PRISM_PROFILE_FUNCTION();

		//We flush whenever we have drawn the max amount of vertices possible for a frame, or when the scene has ended.
		uint32_t dataSize = (uint8_t*)s_Data.m_QuadVertexBufferPointer - (uint8_t*)s_Data.m_QuadVertexBufferBase; //uint8_t is 1 Byte. Thus, when we subtract, we get the amount of elements it takes up in terms of bytes. 
		s_Data.m_QuadVertexBuffer->SetData(s_Data.m_QuadVertexBufferBase, dataSize);
		FlushRenderer();
	}

	void Renderer2D::FlushRenderer()
	{
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray, s_Data.m_QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, const glm::vec4& quadColor)
	{
		DrawQuad({quadPosition.x, quadPosition.y, 0.0f}, quadSize, quadColor);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, const glm::vec4& quadColor)
	{
		PRISM_PROFILE_FUNCTION();

		s_Data.m_QuadVertexBufferPointer->m_QuadPosition = quadPosition;
		s_Data.m_QuadVertexBufferPointer->m_QuadColor = quadColor;
		s_Data.m_QuadVertexBufferPointer->m_TexCoord = { 0.0f, 0.0f, };
		s_Data.m_QuadVertexBufferPointer++;

		s_Data.m_QuadVertexBufferPointer->m_QuadPosition = { quadPosition.x + quadSize.x, quadPosition.y, 0.0f };
		s_Data.m_QuadVertexBufferPointer->m_QuadColor = quadColor;
		s_Data.m_QuadVertexBufferPointer->m_TexCoord = { 1.0f, 0.0f, };
		s_Data.m_QuadVertexBufferPointer++;

		s_Data.m_QuadVertexBufferPointer->m_QuadPosition = { quadPosition.x + quadSize.x, quadPosition.y + quadSize.y, 0.0f };
		s_Data.m_QuadVertexBufferPointer->m_QuadColor = quadColor;
		s_Data.m_QuadVertexBufferPointer->m_TexCoord = { 1.0f, 0.0f, };
		s_Data.m_QuadVertexBufferPointer++;

		s_Data.m_QuadVertexBufferPointer->m_QuadPosition = { quadPosition.x, quadPosition.y + quadSize.y, 0.0f };
		s_Data.m_QuadVertexBufferPointer->m_QuadColor = quadColor;
		s_Data.m_QuadVertexBufferPointer->m_TexCoord = { 0.0f, 1.0f, };
		s_Data.m_QuadVertexBufferPointer++;

		s_Data.m_QuadIndexCount += 6;
		/*s_Data.m_TextureShader->SetShaderFloat4("u_Color", quadColor);
		s_Data.m_TextureShader->SetShaderFloat("u_TilingFactor", 1.0f);
		s_Data.m_WhiteTexture->BindTexture();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), quadPosition) * glm::scale(glm::mat4(1.0f), {quadSize.x, quadSize.y, 1.0f});
		s_Data.m_TextureShader->SetShaderMat4("u_Transform", transform);

		s_Data.m_QuadVertexArray->BindVertexArray();
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray);*/
	}

	//======== Texture ========

	void Renderer2D::DrawQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, const Reference<Texture2D>& quadTexture, float quadTilingFactor, const glm::vec4& quadTintColor)
	{
		DrawQuad({ quadPosition.x, quadPosition.y, 0.0f }, quadSize, quadTexture, quadTilingFactor, quadTintColor);
	}

	//Would be easier to have a QuadProperties struct that has all the properties such as tiling factor filled out so we don't have to constantly set them while drawing - except position. 
	void Renderer2D::DrawQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, const Reference<Texture2D>& quadTexture, float quadTilingFactor, const glm::vec4& quadTintColor)
	{

		/*s_Data.m_TextureShader->SetShaderFloat("u_TilingFactor", quadTilingFactor);
		quadTexture->BindTexture();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), quadPosition) * glm::scale(glm::mat4(1.0f), { quadSize.x, quadSize.y, 1.0f });
		s_Data.m_TextureShader->SetShaderMat4("u_Transform", transform);

		s_Data.m_QuadVertexArray->BindVertexArray();
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray);*/
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, float quadRotation, const glm::vec4& quadColor)
	{
		DrawRotatedQuad({ quadPosition.x, quadPosition.y, 0.0f }, quadSize, quadRotation, quadColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, float quadRotation, const glm::vec4& quadColor)
	{
		PRISM_PROFILE_FUNCTION();

		s_Data.m_TextureShader->SetShaderFloat4("u_Color", quadColor);
		s_Data.m_TextureShader->SetShaderFloat("u_TilingFactor", 1.0f);
		s_Data.m_WhiteTexture->BindTexture();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), quadPosition) * glm::rotate(glm::mat4(1.0f), quadRotation, {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), { quadSize.x, quadSize.y, 1.0f });
		s_Data.m_TextureShader->SetShaderMat4("u_Transform", transform);

		s_Data.m_QuadVertexArray->BindVertexArray();
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, float quadRotation, const Reference<Texture2D>& quadTexture, float quadTilingFactor, const glm::vec4& quadTintColor)
	{
		DrawRotatedQuad({ quadPosition.x, quadPosition.y, 0.0f }, quadSize, quadRotation, quadTexture, quadTilingFactor, quadTintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, float quadRotation, const Reference<Texture2D>& quadTexture, float quadTilingFactor, const glm::vec4& quadTintColor)
	{
		PRISM_PROFILE_FUNCTION();

		s_Data.m_TextureShader->SetShaderFloat4("u_Color", quadTintColor);
		s_Data.m_TextureShader->SetShaderFloat("u_TilingFactor", quadTilingFactor);
		quadTexture->BindTexture();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), quadPosition) * glm::rotate(glm::mat4(1.0f), quadRotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { quadSize.x, quadSize.y, 1.0f });
		s_Data.m_TextureShader->SetShaderMat4("u_Transform", transform);

		s_Data.m_QuadVertexArray->BindVertexArray();
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray);
	}
}