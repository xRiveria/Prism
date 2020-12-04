#include "IconProvider.h"

namespace Prism
{
	static Thumbnail g_NoThumbnail;

	IconProvider::IconProvider()
	{
		m_Application = nullptr;
	}

	IconProvider::~IconProvider()
	{
		m_Thumbnails.clear();
	}

	void Prism::IconProvider::Initialize(Application* application)
	{
		m_Application = application;
		const std::string dataDirectory = WindowsFileSystem::GetProjectDirectory();

		//Load Standard Icons
		LoadThumbnail(dataDirectory + "icons/console_info.png", Icon_Console_Info);
		LoadThumbnail(dataDirectory + "icons/console_warning.png", Icon_Console_Warning);
		LoadThumbnail(dataDirectory + "icons/console_error.png", Icon_Console_Error);
								
		//LoadThumbnail(dataDirectory + "icons/button_play.png", Icon_Button_Play);
		//LoadThumbnail(dataDirectory + "icons/profiler.png", Icon_Profiler);
		//LoadThumbnail(dataDirectory + "icons/resource_cache.png", Icon_ResourceCache);

		LoadThumbnail(dataDirectory + "icons/file.png", Thumbnail_File_Default);
		LoadThumbnail(dataDirectory + "icons/folder.png", Thumbnail_Folder);
		LoadThumbnail(dataDirectory + "icons/audio.png", Thumbnail_File_Audio);
		LoadThumbnail(dataDirectory + "icons/model.png", Thumbnail_File_Model);
		LoadThumbnail(dataDirectory + "icons/scene.png", Thumbnail_File_Scene);
		LoadThumbnail(dataDirectory + "icons/material.png", Thumbnail_File_Material);
		LoadThumbnail(dataDirectory + "icons/shader.png", Thumbnail_File_Shader);
		LoadThumbnail(dataDirectory + "icons/xml.png", Thumbnail_File_Xml);
		LoadThumbnail(dataDirectory + "icons/dll.png", Thumbnail_File_Dll);
		LoadThumbnail(dataDirectory + "icons/txt.png", Thumbnail_File_Txt);
		LoadThumbnail(dataDirectory + "icons/ini.png", Thumbnail_File_Ini);
		LoadThumbnail(dataDirectory + "icons/exe.png", Thumbnail_File_Exe);
		LoadThumbnail(dataDirectory + "icons/script.png", Thumbnail_File_Script);
		LoadThumbnail(dataDirectory + "icons/font.png", Thumbnail_File_Font);
	}

	Reference<Texture2D>IconProvider::GetTextureByType(IconType iconType)
	{
		return LoadThumbnail("", iconType).m_IconTexture;
	}

	Reference<Texture2D>IconProvider::GetTextureByFilePath(const std::string& filePath)
	{
		return LoadThumbnail(filePath).m_IconTexture;
	}

	Reference<Texture2D>IconProvider::GetTextureByThumbnail(const Thumbnail& thumbnail)
	{
		for (const auto& storedThumbnail : m_Thumbnails)
		{
			if (storedThumbnail.m_IconTexture->GetTextureID() == thumbnail.m_IconTexture->GetTextureID())
			{
				return thumbnail.m_IconTexture;
			}
		}

		return nullptr;
	}

	const Thumbnail& IconProvider::LoadThumbnail(const std::string& filePath, IconType iconType, int size)
	{
		//Check if we already have this thumbnail in our vector by type.
		if (iconType != Thumbnail_Custom) 
		{
			for (const auto& thumbnail : m_Thumbnails)
			{
				if (thumbnail.m_IconType == iconType)
				{
					return thumbnail;
				}
			}
		}
		else //If we don't have this thumbnail by type, we check if we already have this thumbnail by path in our vector.
		{
			for (const auto& thumbnail : m_Thumbnails)
			{
				if (thumbnail.m_FilePath == filePath)
				{
					return thumbnail;
				}
			}
		}

		//If we cannot locate the thumbnail in our thumbnails collection at all, we do some manual searches. 

		//Directory
		if (WindowsFileSystem::IsDirectoryValid(filePath)) { return GetThumbnailByType(IconType::Thumbnail_Folder); }
		//Model
		if (WindowsFileSystem::IsSupportedModelFile(filePath)) { return GetThumbnailByType(IconType::Thumbnail_File_Model); }
		//Audio
		if (WindowsFileSystem::IsSupportedAudioFile(filePath)) { return GetThumbnailByType(IconType::Thumbnail_File_Audio); }
		//Shader
		if (WindowsFileSystem::IsSupportedShaderFile(filePath)) { return GetThumbnailByType(IconType::Thumbnail_File_Shader); }
		//Font
		if (WindowsFileSystem::IsSupportedFontFile(filePath)) { return GetThumbnailByType(IconType::Thumbnail_File_Font); }
		//Scene
		if (WindowsFileSystem::IsEngineSceneFile(filePath)) { return GetThumbnailByType(IconType::Thumbnail_File_Scene); }
		//Script
		if (WindowsFileSystem::IsEngineScriptFile(filePath)) { return GetThumbnailByType(IconType::Thumbnail_File_Script); }

		//Xml
		if (WindowsFileSystem::GetExtensionFromFilePath(filePath) == ".xml") { return GetThumbnailByType(IconType::Thumbnail_File_Xml); }
		//Dll
		if (WindowsFileSystem::GetExtensionFromFilePath(filePath) == ".dll") { return GetThumbnailByType(IconType::Thumbnail_File_Dll); }
		//Txt
		if (WindowsFileSystem::GetExtensionFromFilePath(filePath) == ".txt") { return GetThumbnailByType(IconType::Thumbnail_File_Txt); }
		//Ini
		if (WindowsFileSystem::GetExtensionFromFilePath(filePath) == ".ini") { return GetThumbnailByType(IconType::Thumbnail_File_Ini); }
		//Exe
		if (WindowsFileSystem::GetExtensionFromFilePath(filePath) == ".exe") { return GetThumbnailByType(IconType::Thumbnail_File_Exe); }

		//Texture
		if (WindowsFileSystem::IsSupportedImageFile(filePath) || WindowsFileSystem::IsEngineTextureFile(filePath))
		{
			//Make a cheap texture and load it asynchronously.
			Reference<Texture2D> texture = Texture2D::CreateTexture(filePath);
			m_Application->GetThreadingLibrary()->AddTask([texture, filePath]()
			{	   	
				std::cout << texture->GetTextureID() << "\n";
			});

			m_Thumbnails.emplace_back(iconType, texture, filePath);

			return m_Thumbnails.back();
		}

		//Finally, if we really cannot find support for the file, we return a blank file thumbnail.
		return GetThumbnailByType(Thumbnail_File_Default);
	}

	const Thumbnail& IconProvider::GetThumbnailByType(IconType iconType)
	{
		for (const auto& thumbnail : m_Thumbnails)
		{
			if (thumbnail.m_IconType == iconType)
			{
				return thumbnail;
			}
		}

		return g_NoThumbnail;
	}
}
