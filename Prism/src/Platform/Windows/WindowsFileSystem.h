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
		static std::string GetExtensionFromFilePath(const std::string& filePath);
		static std::string GetFileNameFromFilePath(const std::string& filePath);
		
		static std::vector<std::string> GetDirectoriesInDirectory(const std::string& directoryPath);
		static std::vector<std::string> GetFilesInDirectory(const std::string& directoryPath);

		inline static std::string GetProjectDirectory() { return "assets/"; }

		//Supported Files
		static bool IsSupportedModelFile(const std::string& filePath);	
		static bool IsEngineSceneFile(const std::string& filePath);
		
		//Supported Files in Directory
		static std::vector<std::string> GetSupportedFilesInDirectory(const std::string& fileDirectory); //Master

		static std::vector<std::string> GetSupportedSceneFilesInDirectory(const std::string& fileDirectory);
		static std::vector<std::string> GetSupportedModelFilesFromPaths(const std::vector<std::string>& filePaths);
		static std::vector<std::string> GetSupportedModelFilesInDirectory(const std::string& fileDirectory);
	};
	
	//Formatting

	static const char* EXTENSION_SCENE = ".prism";

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


}