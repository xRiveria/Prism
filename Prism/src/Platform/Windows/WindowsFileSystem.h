#pragma once
#include <vector>
#include <string>

namespace Prism
{
	class WindowsFileSystem
	{
	public:
		//File Creation
		static void CreateTextFile(const std::string& filePath, const std::string& text);

		//Strings
		static std::string ConvertStringToUppercase(const std::string& lowercaseString);
		static std::wstring ConvertStringToWstring(const std::string& string);

		//Paths
		static void OpenDirectoryWindow(const std::string& directoryPath);
		static bool IsDirectoryValid(const std::string& directoryPath);
		static bool Delete(const std::string& filePath);
		static std::string GetExtensionFromFilePath(const std::string& filePath);
		static std::string GetFileNameFromFilePath(const std::string& filePath);
		
		static std::vector<std::string> GetDirectoriesInDirectory(const std::string& directoryPath);
		static std::vector<std::string> GetFilesInDirectory(const std::string& directoryPath);

		inline static std::string GetProjectDirectory() { return "assets/"; }

		//Supported Files
		static bool IsSupportedModelFile(const std::string& filePath);	
		static bool IsSupportedAudioFile(const std::string& filePath);
		static bool IsSupportedShaderFile(const std::string& filePath);
		static bool IsEngineSceneFile(const std::string& filePath);
		static bool IsEngineScriptFile(const std::string& filePath);
		static bool IsEngineTextureFile(const std::string& filePath);
		static bool IsEngineModelFile(const std::string& filePath);
		static bool IsEngineMaterialFile(const std::string& filePath);
		static bool IsSupportedFontFile(const std::string& filePath);
		static bool IsSupportedImageFile(const std::string& filePath);
		
		//Supported Files in Directory
		static std::vector<std::string> GetSupportedFilesInDirectory(const std::string& fileDirectory); //Master

		static std::vector<std::string> GetSupportedSceneFilesInDirectory(const std::string& fileDirectory);
		static std::vector<std::string> GetSupportedModelFilesFromPaths(const std::vector<std::string>& filePaths);
		static std::vector<std::string> GetSupportedModelFilesInDirectory(const std::string& fileDirectory);
	};
	
	//Formatting

	static const char* EXTENSION_SCENE = ".prism";
	static const char* EXTENSION_MATERIAL = ".material";
	static const char* EXTENSION_MODEL = ".model";
	static const char* EXTENSION_PREFAB = ".prefab";
	static const char* EXTENSION_SHADER = ".shader";
	static const char* EXTENSION_FONT = ".ttf";
	static const char* EXTENSION_TEXTURE = ".texture";
	static const char* EXTENSION_MESH = ".mesh";
	static const char* EXTENSION_AUDIO = ".audio";
	static const char* EXTENSION_SCRIPT = ".cs";

	static const std::vector<std::string> SupportedFormats_Model
	{
		".3ds",
		".obj",
		".fbx",
		".blend",
		".dae",
		".gltf",
		".lwo",
		".c4d",
		".ase",
		".dxf",
		".hmp",
		".md2",
		".md3",
		".md5",
		".mdc",
		".mdl",
		".nff",
		".ply",
		".stl",
		".x",
		".smd",
		".lxo",
		".lws",
		".ter",
		".ac3d",
		".ms3d",
		".cob",
		".q3bsp",
		".xgl",
		".csm",
		".bvh",
		".b3d",
		".ndo"
	};

	static const std::vector<std::string> SupportedFormats_Audio
	{
		".aiff",
		".asf",
		".asx",
		".dls",
		".flac",
		".fsb",
		".it",
		".m3u",
		".midi",
		".mod",
		".mp2",
		".mp3",
		".ogg",
		".pls",
		".s3m",
		".vag", // PS2/PSP
		".wav",
		".wax",
		".wma",
		".xm",
		".xma" // XBOX 360
	};

	static const std::vector<std::string> SupportedFormats_Image
	{
		".jpg",
		".png",
		".bmp",
		".tga",
		".dds",
		".exr",
		".raw",
		".gif",
		".hdr",
		".ico",
		".iff",
		".jng",
		".jpeg",
		".koala",
		".kodak",
		".mng",
		".pcx",
		".pbm",
		".pgm",
		".ppm",
		".pfm",
		".pict",
		".psd",
		".raw",
		".sgi",
		".targa",
		".tiff",
		".tif", // tiff can also be tif
		".wbmp",
		".webp",
		".xbm",
		".xpm"
	};

	static const std::vector<std::string> SupportedFormats_Shader
	{
		".hlsl",
		".glsl"
	};

	static const std::vector<std::string> SupportedFormats_Scripts
	{
		".cs"
	};


}