#include "PrismPrecompiledHeader.h"
#include "WindowsFileSystem.h"
#include <filesystem>
#include <locale>
#include <shellapi.h>

namespace Prism
{
	void WindowsFileSystem::CreateTextFile(const std::string& filePath, const std::string& text)
	{

	}

	std::string WindowsFileSystem::ConvertStringToUppercase(const std::string& lowercaseString)
	{
		const std::locale locale;
		std::string uppercaseText;

		for (const auto& character : lowercaseString)
		{
			uppercaseText += std::toupper(character, locale);
		}

		return uppercaseText;
	}

	std::wstring WindowsFileSystem::ConvertStringToWstring(const std::string& string)
	{
		const auto slength = static_cast<int>(string.length()) + 1;
		const auto len = MultiByteToWideChar(CP_ACP, 0, string.c_str(), slength, nullptr, 0);
		const auto buf = new wchar_t[len];

		MultiByteToWideChar(CP_ACP, 0, string.c_str(), slength, buf, len);
		std::wstring result(buf);

		delete[] buf;

		return result;
	}

	void WindowsFileSystem::OpenDirectoryWindow(const std::string& directoryPath)
	{
		ShellExecute(nullptr, nullptr, ConvertStringToWstring(directoryPath).c_str(), nullptr, nullptr, SW_SHOW);
	}

	bool WindowsFileSystem::IsDirectoryValid(const std::string& directoryPath)
	{
		try
		{
			if (std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath))
			{
				return true;
			}
		}

		catch (std::filesystem::filesystem_error& caughtError)
		{
			std::string displayedWarning = std::string("Invalid Path: ") + directoryPath.c_str();
			PRISM_EDITOR_WARN(displayedWarning);
		}

		return false;
	}

	std::string WindowsFileSystem::GetExtensionFromFilePath(const std::string& filePath)
	{
		std::string fileExtension;

		//A system error is possible if the characters are something that cannot be converted, like Russian.
		try
		{
			fileExtension = std::filesystem::path(filePath).extension().generic_string();
		}
		catch (std::system_error& error)
		{
			PRISM_CLIENT_WARN("Failed to get extension of {0}", error.what());
		}

		return fileExtension;
	}

	std::string WindowsFileSystem::GetFileNameFromFilePath(const std::string& filePath)
	{
		return std::filesystem::path(filePath).filename().generic_string();
	}

	std::vector<std::string> WindowsFileSystem::GetDirectoriesInDirectory(const std::string& directoryPath)
	{
		std::vector<std::string> directories;

		const std::filesystem::directory_iterator iteratorEnd; //Default construction yields past the end.
		for (std::filesystem::directory_iterator iterator(directoryPath); iterator != iteratorEnd; ++iterator)
		{
			if (!std::filesystem::is_directory(iterator->status()))
			{
				continue;
			}

			std::string path;

			//A system error is possible if the characters are something that cannot be converted, like Russian.
			try
			{
				path = iterator->path().string();
			}
			catch (std::system_error& error)
			{
				PRISM_CLIENT_WARN("Failed to read a directory path of {0}", error.what());
			}

			if (!path.empty())
			{
				//Finally, Save.
				directories.emplace_back(path);
			}
		}

		return directories;
	}

	std::vector<std::string> WindowsFileSystem::GetFilesInDirectory(const std::string& directoryPath)
	{
		std::vector<std::string> filePaths;

		const std::filesystem::directory_iterator iteratorEnd; //Default construction yields past the ened.
		for (std::filesystem::directory_iterator iterator(directoryPath); iterator != iteratorEnd; ++iterator)
		{
			if (!std::filesystem::is_regular_file(iterator->status()))
			{
				continue;
			}

			try
			{
				//A system error is possible if the characters are something that cannot be converted, like Russian.
				filePaths.emplace_back(iterator->path().string());
			}
			catch (std::system_error& error)
			{
				PRISM_CLIENT_WARN("Failed to read the file path of {0}", error.what());
			}
		}

		return filePaths;
	}

	bool WindowsFileSystem::IsSupportedModelFile(const std::string& filePath)
	{
		const std::string fileExtension = GetExtensionFromFilePath(filePath);

		for (const auto& format : SupportedFormats_Model)
		{
			if (fileExtension == format || fileExtension == ConvertStringToUppercase(format))
			{
				return true;
			}
		}

		return false;
	}

	bool WindowsFileSystem::IsEngineSceneFile(const std::string& filePath)
	{
		return GetExtensionFromFilePath(filePath) == EXTENSION_SCENE;
	}

	std::vector<std::string> WindowsFileSystem::GetSupportedFilesInDirectory(const std::string& fileDirectory)
	{
		return std::vector<std::string>();
	}

	std::vector<std::string> WindowsFileSystem::GetSupportedSceneFilesInDirectory(const std::string& fileDirectory)
	{
		std::vector<std::string> sceneFiles;

		std::vector<std::string> files = GetFilesInDirectory(fileDirectory);
		for (const auto& file : files)
		{
			if (!IsEngineSceneFile(fileDirectory))
			{
				continue;
			}
			sceneFiles.emplace_back(file);
		}

		return sceneFiles;
	}

	std::vector<std::string> WindowsFileSystem::GetSupportedModelFilesFromPaths(const std::vector<std::string>& filePaths)
	{
		std::vector<std::string> modelFiles;

		for (const auto& path : filePaths)
		{
			if (!IsSupportedModelFile(path))
			{
				continue;
			}
			modelFiles.emplace_back(path);
		}

		return modelFiles;
	}

	std::vector<std::string> WindowsFileSystem::GetSupportedModelFilesInDirectory(const std::string& fileDirectory)
	{
		return GetSupportedModelFilesFromPaths(GetFilesInDirectory(fileDirectory));
	}

}