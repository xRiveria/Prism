#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"
#include "Prism/Renderer/SubTexture2D.h"

namespace Prism
{
	class Renderer2D
	{
	public:
		static void Initialize2DRenderer();
		static void Shutdown2DRenderer();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void FlushRenderer();

		//Primitives
		static void DrawQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, const glm::vec4& quadColor);
		static void DrawQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, const glm::vec4& quadColor); //We use Vec3 here because we can possibly use the last Z value for sorting, such as rendering certain things at the back of the scene - almost layering of sorts.
		static void DrawQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, const Reference<Texture2D>& quadTexture, float quadTilingFactor = 1.0f, const glm::vec4& quadTintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, const Reference<Texture2D>& quadTexture, float quadTilingFactor = 1.0f, const glm::vec4& quadTintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, const Reference<SubTexture2D>& quadSubTexture, float quadTilingFactor = 1.0f, const glm::vec4& quadTintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, const Reference<SubTexture2D>& quadSubTexture, float quadTilingFactor = 1.0f, const glm::vec4& quadTintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, float quadRotation, const glm::vec4& quadColor);
		static void DrawRotatedQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, float quadRotation, const glm::vec4& quadColor);
		static void DrawRotatedQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, float quadRotation, const Reference<Texture2D>& quadTexture, float quadTilingFactor = 1.0f, const glm::vec4& quadTintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, float quadRotation, const Reference<Texture2D>& quadTexture, float quadTilingFactor = 1.0f, const glm::vec4& quadTintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& quadPosition, const glm::vec2& quadSize, float quadRotation, const Reference<SubTexture2D>& quadSubTexture, float quadTilingFactor = 1.0f, const glm::vec4& quadTintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& quadPosition, const glm::vec2& quadSize, float quadRotation, const Reference<SubTexture2D>& quadSubTexture, float quadTilingFactor = 1.0f, const glm::vec4& quadTintColor = glm::vec4(1.0f));
		
		//Statistics

		struct Statistics
		{
			uint32_t m_DrawCalls = 0;
			uint32_t m_QuadCount = 0;

			uint32_t GetTotalVertexCount() { return m_QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return m_QuadCount * 6; }
		};

		static Statistics GetBatchingStatistics();
		static void ResetBatchingStatistics();

	private:
		static void FlushAndReset();
	};
}
