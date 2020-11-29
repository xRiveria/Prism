#pragma once
#include "Prism.h"

namespace Prism
{
	enum IconType
	{
		Icon_NotAssigned,
		Icon_Console_Info,
		Icon_Console_Warning,
		Icon_Console_Error,
		Icon_Console_Play,

		Thumbnail_Custom,
		Thumbnail_Folder,
		Thumbnail_File_Audio,
		Thumbnail_File_Scene,
		Thumbnail_File_Model,
		Thumbnail_File_Default,
		Thumbnail_File_Material,
		Thumbnail_File_Shader,
		Thumbnail_File_Xml,
		Thumbnail_File_Txt,
		Thumbnail_File_Ini,
		Thumbnail_File_Exe,
		Thumbnail_File_Script,
		Thumbnail_File_Font
	};

	//Information on any file's thumbnail and its associated path.
	struct Thumbnail
	{
		Thumbnail() = default;
		Thumbnail(IconType iconType, Reference<Texture2D> iconTexture, const std::string& filePath)
		{
			m_IconType = iconType;
			m_IconTexture = iconTexture;
			m_FilePath = filePath;
		}

		IconType m_IconType = IconType::Icon_NotAssigned;
		Reference<Texture2D> m_IconTexture;
		std::string m_FilePath;
	};
	
	class IconProvider
	{
	public:
		static IconProvider& GetInstance()
		{
			static IconProvider iconProviderInstance;
			return iconProviderInstance;
		}

		void Initialize();

		//Ways to obtain icon texture. 
		Reference<Texture2D> GetTextureByType(IconType iconType);
		Reference<Texture2D> GetTextureByFilePath(const std::string& filePath);
		Reference<Texture2D> GetTextureByThumbnail(const Thumbnail& thumbnail);

		const Thumbnail& LoadThumbnail(const std::string& filePath, IconType iconType = IconType::Thumbnail_Custom, int size = 100);

	private:
		IconProvider();
		~IconProvider();

	private:
		const Thumbnail& GetThumbnailByType(IconType iconType);
		std::vector<Thumbnail> m_Thumbnails;
	};


}