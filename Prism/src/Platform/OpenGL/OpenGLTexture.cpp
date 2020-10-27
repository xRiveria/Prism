#include "PrismPrecompiledHeader.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

namespace Prism
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t textureWidth, uint32_t textureHeight) : m_TextureWidth(textureWidth), m_TextureHeight(textureHeight)
	{
		PRISM_PROFILE_FUNCTION();

		m_TextureInternalOpenGLFormat = GL_RGBA8;
		m_TextureImageDataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, m_TextureInternalOpenGLFormat, m_TextureWidth, m_TextureHeight);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string filePath) : m_FilePath(filePath)
	{
		PRISM_PROFILE_FUNCTION();

		int textureWidth, textureHeight, textureChannels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* imageData = nullptr;
		{
			PRISM_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath)"); //Might be a good idea to log width, height and channels as well. 
			imageData = stbi_load(filePath.c_str(), &textureWidth, &textureHeight, &textureChannels, 0);
		}

		PRISM_ENGINE_ASSERT(imageData, "Failed to load Image!");

		m_TextureWidth = textureWidth;
		m_TextureHeight = textureHeight;

		if (textureChannels == 4)
		{
			m_TextureInternalOpenGLFormat = GL_RGBA8; //OpenGL storage format.
			m_TextureImageDataFormat = GL_RGBA; //Image data format. 
		}
		else if (textureChannels == 3)
		{
			m_TextureInternalOpenGLFormat = GL_RGB8; 
			m_TextureImageDataFormat = GL_RGB;
		}

		PRISM_ENGINE_ASSERT(m_TextureInternalOpenGLFormat && m_TextureImageDataFormat, "Image format not support.");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, m_TextureInternalOpenGLFormat, m_TextureWidth, m_TextureHeight);
		
		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_TextureWidth, m_TextureHeight, m_TextureImageDataFormat, GL_UNSIGNED_BYTE, imageData);
	
		stbi_image_free(imageData);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		PRISM_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLTexture2D::SetTextureData(void* textureData, uint32_t size)
	{
		PRISM_PROFILE_FUNCTION();

		uint32_t bitsPerPixel = m_TextureImageDataFormat == GL_RGBA ? 4 : 3;
		PRISM_ENGINE_ASSERT(size == m_TextureWidth * m_TextureHeight * bitsPerPixel, "Texture Data must be entire texture!");
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_TextureWidth, m_TextureHeight, m_TextureImageDataFormat, GL_UNSIGNED_BYTE, textureData);
	}

	void OpenGLTexture2D::BindTexture(uint32_t textureSlot) const
	{
		PRISM_PROFILE_FUNCTION();

		glBindTextureUnit(textureSlot, m_TextureID);
	}
}