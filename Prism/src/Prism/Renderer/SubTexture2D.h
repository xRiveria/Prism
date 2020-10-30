#pragma once
#include "Texture.h"
#include "glm/glm.hpp"

namespace Prism
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Reference<Texture2D>& texture, const glm::vec2& subTextureMinimumBounds, const glm::vec2& subTextureMaximumBounds);
		
		const Reference<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTextureCoordinates() const { return m_TextureCoordinates; }

		static Reference<SubTexture2D> CreateFromCoords(const Reference<Texture2D>& texture, const glm::vec2& spriteCoordinates, const glm::vec2& spriteCellSize, const glm::vec2& spriteSize = { 1, 1 });

	private:
		Reference<Texture2D> m_Texture;
		glm::vec2 m_TextureCoordinates[4];
	};
}
