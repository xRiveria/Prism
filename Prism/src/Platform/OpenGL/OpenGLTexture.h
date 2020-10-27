#pragma once
#include "Prism/Renderer/Texture.h"
#include "glad/glad.h"

namespace Prism
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t textureWidth, uint32_t textureHeight);
		OpenGLTexture2D(const std::string filePath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetTextureWidth() const override { return m_TextureWidth; }
		virtual uint32_t GetTextureHeight() const override { return m_TextureHeight; }

		virtual void SetTextureData(void* textureData, uint32_t size);

		virtual void BindTexture(uint32_t textureSlot = 0) const override;

	private:
		std::string m_FilePath; //We keep this here for hot reloading textures (recreating textures when the file is changed outside the engine).
		uint32_t m_TextureWidth, m_TextureHeight;
		uint32_t m_TextureID;
		GLenum m_TextureInternalOpenGLFormat;
		GLenum m_TextureImageDataFormat;
	};
}


