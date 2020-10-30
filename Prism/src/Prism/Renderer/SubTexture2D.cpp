#include "PrismPrecompiledHeader.h"
#include "SubTexture2D.h"

namespace Prism
{
	SubTexture2D::SubTexture2D(const Reference<Texture2D>& texture, const glm::vec2& subTextureMinimumBounds, const glm::vec2& subTextureMaximumBounds) : m_Texture(texture)
	{
		m_TextureCoordinates[0] = { subTextureMinimumBounds.x, subTextureMinimumBounds.y };  //Bottom Left
		m_TextureCoordinates[1] = { subTextureMaximumBounds.x, subTextureMinimumBounds.y };  //Bottom Right
		m_TextureCoordinates[2] = { subTextureMaximumBounds.x, subTextureMaximumBounds.y };  //Upper Right
		m_TextureCoordinates[3] = { subTextureMinimumBounds.x, subTextureMaximumBounds.y };  //Upper Left
	}

	Reference<SubTexture2D> SubTexture2D::CreateFromCoords(const Reference<Texture2D>& texture, const glm::vec2& spriteCoordinates, const glm::vec2& spriteCellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 minimumBounds = { (spriteCoordinates.x * spriteCellSize.x) / texture->GetTextureWidth(), (spriteCoordinates.y * spriteCellSize.y) / texture->GetTextureHeight() };
		glm::vec2 maximumBounds = { ((spriteCoordinates.x + spriteSize.x) * spriteCellSize.x) / texture->GetTextureWidth(), ((spriteCoordinates.y + spriteSize.y) * spriteCellSize.y) / texture->GetTextureHeight() };
		
		return CreateReference<SubTexture2D>(texture, minimumBounds, maximumBounds);
				
	}
}