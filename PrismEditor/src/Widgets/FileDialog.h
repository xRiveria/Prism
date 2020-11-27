#pragma once
#include "Prism.h"
#include <string>
#include <vector>
#include <filesystem>

namespace Prism
{
	enum FileDialogType
	{
		FileDialogType_Browser,
		FilaDialogType_FileSelection
	};

	enum FileDialogOperation
	{
		FileDialogOperation_Open,
		FileDialogOperation_Load,
		FileDialogOperation_Save
	};

	enum FileDialogFilter
	{
		FileDialogFilter_All,
		FileDialogFilter_Scene,
		FileDialogFilter_Model
	};

	class FileDialogNavigation
	{
	public:
		bool IsDirectoryValid(const std::string& path)
		{
			try
			{
				if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
				{
					return true;
				}
			}

			catch (std::filesystem::filesystem_error& caughtError)
			{
				std::string displayedWarning = std::string("Invalid Path: ") + path.c_str();
				PRISM_EDITOR_WARN(displayedWarning);
			}
		}

		bool Navigate(std::string directory, const bool& updateTrackedHistory = true)
		{
			if (!IsDirectoryValid(directory))
			{
				return false;
			}

			//If the directly ends with a slash, remove it (simplifies things below).
			if (directory.back() == '/')
			{
				directory = directory.substr(0, directory.size() - 1); //Removes the last trailing / character.
			}

			//If the path we are navigating to is the current directory, we do nothing.
			if (m_CurrentPath == directory)
			{
				return false;
			}

			//Update current path.
			m_CurrentPath = directory;

			//Update our navigated history if enabled.
			if (updateTrackedHistory)
			{

			}
		}

		bool ForwardDirectory()
		{
			if (m_PathHistory.empty() || (m_PathHistoryIndex + 1) >= static_cast<int>(m_PathHistory.size())) //If the user's path history is empty, or if the next directory is somehow invalid.
			{
				return false;
			}

			Navigate(m_PathHistory[++m_PathHistoryIndex], false); //We navigate first, and then increment our path history index.

			return true;
		}

		bool BackwardDirectory()
		{
			if (m_PathHistory.empty() || (m_PathHistoryIndex - 1) < 0)  //If the user's path history is empty, or the previous directory is somehow invalid.
			{
				return false;
			}

			Navigate(m_PathHistory[--m_PathHistoryIndex], false); //We navigate first, and then decrement our path history index.
			
			return true;
		}

		std::string m_CurrentPath;
		std::vector<std::string> m_PathHierarchy;
		std::vector<std::string> m_PathHierarchyLabels;
		std::vector<std::string> m_PathHistory; //Track user path navigations. We only do this for manual navigations that are not Forward(), Backward().
		int m_PathHistoryIndex = -1;
	};

}
