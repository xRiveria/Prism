#pragma once
#include "Prism/Renderer/Texture.h"

namespace Prism
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string filePath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetTextureWidth() const override { return m_TextureWidth; }
		virtual uint32_t GetTextureHeight() const override { return m_TextureHeight; }

		virtual void BindTexture(uint32_t textureSlot = 0) const override;

	private:
		std::string m_FilePath; //We keep this here for hot reloading textures (recreating textures when the file is changed outside the engine).
		uint32_t m_TextureWidth, m_TextureHeight;
		uint32_t m_TextureID;
	};
}


