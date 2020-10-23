#include "PrismPrecompiledHeader.h"
#include "OpenGLTexture.h"
#include "glad/glad.h"
#include "stb_image.h"

namespace Prism
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string filePath) : m_FilePath(filePath)
	{
		int textureWidth, textureHeight, textureChannels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* imageData = stbi_load(filePath.c_str(), &textureWidth, &textureHeight, &textureChannels, 0);
		PRISM_ENGINE_ASSERT(imageData, "Failed to load Image!");

		m_TextureWidth = textureWidth;
		m_TextureHeight = textureHeight;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, GL_RGB8, m_TextureWidth, m_TextureHeight);
		
		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_TextureWidth, m_TextureHeight, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	
		stbi_image_free(imageData);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLTexture2D::BindTexture(uint32_t textureSlot) const
	{
		glBindTextureUnit(textureSlot, m_TextureID);
	}
}