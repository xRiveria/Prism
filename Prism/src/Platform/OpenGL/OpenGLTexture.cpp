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

		GLenum internalOpenGLFormat = 0, imageDataFormat = 0;
		if (textureChannels == 4)
		{
			internalOpenGLFormat = GL_RGBA8; //OpenGL storage format.
			imageDataFormat = GL_RGBA; //Image data format. 
		}
		else if (textureChannels == 3)
		{
			internalOpenGLFormat = GL_RGB8; 
			imageDataFormat = GL_RGB;
		}

		PRISM_ENGINE_ASSERT(internalOpenGLFormat && imageDataFormat, "Image format not support.");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, internalOpenGLFormat, m_TextureWidth, m_TextureHeight);
		
		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_TextureWidth, m_TextureHeight, imageDataFormat, GL_UNSIGNED_BYTE, imageData);
	
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