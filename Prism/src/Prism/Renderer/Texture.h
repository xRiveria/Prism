#pragma once
#include "Prism/Core/Core.h"
#include <string>

namespace Prism
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetTextureWidth() const = 0;
		virtual uint32_t GetTextureHeight() const = 0;
		virtual uint32_t GetTextureID() const = 0;

		virtual void SetTextureData(void* textureData, uint32_t size) = 0;

		virtual void BindTexture(uint32_t textureSlot = 0) const = 0;

		virtual bool operator==(const Texture& otherTexture) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Reference<Texture2D> CreateTexture(uint32_t textureWidth, uint32_t textureHeight);
		static Reference<Texture2D> CreateTexture(const std::string& filePath);
	};
}
