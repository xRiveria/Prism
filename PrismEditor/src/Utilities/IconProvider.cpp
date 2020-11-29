#include "IconProvider.h"

namespace Prism
{
	static Thumbnail g_NoThumbnail;

	IconProvider::~IconProvider()
	{
		m_Thumbnails.clear();
	}

	void Prism::IconProvider::Initialize()
	{

	}

	Reference<Texture2D> IconProvider::GetTextureByType(IconType iconType)
	{
		return LoadThumbnail("", iconType).m_IconTexture;
	}

	Reference<Texture2D>IconProvider::GetTextureByFilePath(const std::string& filePath)
	{
		return LoadThumbnail(filePath).m_IconTexture;
	}

	Reference<Texture2D> IconProvider::GetTextureByThumbnail(const Thumbnail& thumbnail)
	{
		return Reference<Texture2D>();
	}

	const Thumbnail& IconProvider::LoadThumbnail(const std::string& filePath, IconType iconType, int size)
	{
		// TODO: insert return statement here
	}

	const Thumbnail& IconProvider::GetThumbnailByType(IconType iconType)
	{
		// TODO: insert return statement here
	}
}
