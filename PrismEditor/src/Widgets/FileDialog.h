#pragma once
#include "Prism.h"
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <functional>
#include <variant>

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

	//============ To Move Out ===================
	//Drag and Drop
	enum DragPayloadType
	{
		DragPayload_Unknown,
		DragPayload_Texture,
		DragPayload_Entity,
		DragPayload_Model,
		DragPayload_Audio,
		DragPayload_Script,
		DragPayload_Material
	};

	struct DragDropPayload
	{
		typedef std::variant<const char*, unsigned int> DataVariant;
		DragDropPayload(const DragPayloadType type = DragPayloadType::DragPayload_Unknown, const DataVariant data = nullptr)
		{
			this->m_Type = type;
			this->m_Data = data;
		}

		DragPayloadType m_Type;
		DataVariant m_Data;
		
	};

	inline static bool IsDirectoryValid(const std::string& path)  //static inline generally works as static, but the inline keyword suggest compiler trying to inline this function.
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

	inline static std::string GetFileNameFromFilePath(const std::string& path)
	{
		return std::filesystem::path(path).filename().generic_string();
	}

	static uint32_t g_GlobalID = 0;

	class GenerateGlobalObjectID
	{
	public:
		static uint32_t GenerateID() { return ++g_GlobalID; }  //Return and increment.
	};


	//=============================================

	//Keeps track of directory navigation.
	class FileDialogNavigation
	{
	public:
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
				m_PathHistory.emplace_back(m_CurrentPath);
				m_PathHistoryIndex++;
			}

			//Clear Hierarchy
			m_PathHierarchy.clear();
			m_PathHierarchyLabels.clear();

			//Is there a slash?
			std::size_t position = m_CurrentPath.find('/'); 

			//If there are no slashes, there is no nesting, and we are done.
			if (position == std::string::npos) //std::string::npos is implementation defined index that is always out of bounds of any std::string instance. 
			{
				m_PathHierarchy.emplace_back(m_CurrentPath);
			}
			else //If there is a slash, get the individual directories between slashes.
			{
				std::size_t previousPosition = 0;
				while (true)
				{
					//Lets save everything we have before the slash.
					m_PathHierarchy.emplace_back(m_CurrentPath.substr(0, position)); //If any slashes are found, it means we have another directory. We save it when the loop returns back here with the updated position.

					//Attempt to find a slash after the one we already found.

					previousPosition = position;
					position = m_CurrentPath.find('/', position + 1); //Returns npos when the pattern isn't found.

					//If there are no more slashes.
					if (position == std::string::npos) //If position reaches npos.
					{
						//Save the complete path to this directory.
						m_PathHierarchy.emplace_back(m_CurrentPath); 
						break;
					}
				}
			}

			//Create a proper looking label to show in the editor for each path. -> To Revamp
			for (const auto& path : m_PathHierarchy)
			{
				position = path.find('/');
				if (position == std::string::npos) 
				{
					m_PathHierarchyLabels.emplace_back(path + " >");
				}
				else
				{
					m_PathHierarchyLabels.emplace_back(path.substr(path.find_last_of('/') + 1) + " >"); //We use > to indicate that there is a next path.
				}
			}

			return true;
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

	class FileDialogItem
	{
	public:
		FileDialogItem(const std::string& path, const Reference<Texture2D> thumbnail)
		{
			m_Path = path;
			m_Thumbnail = thumbnail;
			m_ID = GenerateGlobalObjectID::GenerateID();
			m_IsDirectory = IsDirectoryValid(path);
			m_Label = GetFileNameFromFilePath(path);
		}

		const std::string& GetPath() const { return m_Path; }
		const std::string& GetLabel() const { return m_Label; }
		const unsigned int& GetID() const { return m_ID; }
		//const Reference<Texture2D> GetTexture() const { return ;
		const bool& IsDirectory() const { return m_IsDirectory; }
		const float& GetTimeSinceLastClickMilliseconds() const { return static_cast<float>(m_TimeSinceLastClick.count()); }

		void Clicked()
		{
			const auto timeNow = std::chrono::high_resolution_clock::now();
			m_TimeSinceLastClick = timeNow - m_LastClickTime;
			m_LastClickTime = timeNow;
		}

	private:
		Reference<Texture2D> m_Thumbnail;
		unsigned int m_ID;
		std::string m_Path;
		std::string m_Label;
		bool m_IsDirectory;
		std::chrono::duration<double, std::milli> m_TimeSinceLastClick;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_LastClickTime;
	};

	class FileDialog
	{
	public:
		FileDialog(bool standaloneWindow, FileDialogType dialogType, FileDialogOperation dialogOperation, FileDialogFilter dialogFilter);

		//Type & Filter
		FileDialogType GetType() const { return m_DialogType; }
		FileDialogFilter GetFilter() const { return m_DialogFilter; }
		
		//Operation
		FileDialogOperation GetOperation() const { return m_DialogOperation; }
		void SetOperation(const FileDialogOperation& operation);

		//Shows the dialog and returns true if a selection was made.
		bool Show(bool* isVisible, std::string* directory = nullptr, std::string* filePath = nullptr);

		void SetCallbackOnItemClicked(const std::function<void(const std::string&)>& callback) { m_CallbackOnItemClicked = callback; }
		void SetCallbackOnItemDoubleClicked(const std::function<void(const std::string&)>& callback) { m_CallbackOnItemDoubleClicked = callback; }

	private:
		void ShowTop(bool* isVisible); 
		void ShowMiddle();
		void ShowBottom(bool* isVisible);
			
		//Item Functionality Handling
		void ItemDrag(FileDialogItem* item) const;
		void ItemClick(FileDialogItem* item) const;
		void ItemContextMenu(FileDialogItem* item) const;

		//Misc
		bool DialogUpdateFromDirectory(const std::string& path);
		void EmptyAreaContextMenu();

		//Options
		const bool m_DropShadow = true;
		const float m_MinimumItemSize = 50.0f;
		const float m_MaximumItemSize = 200.0f;
		const glm::vec4 m_ContentBackground = glm::vec4(0.0f, 0.0f, 0.0f, 50.0f);

		//Flags
		bool m_IsWindow;
		bool m_SelectionMade;
		bool m_IsDirty;
		bool m_IsHoveringItem;
		bool m_IsHoveringWindow;

		std::string m_Title; //Window Title
		FileDialogNavigation m_Navigation; //Navigation Context
		std::string m_InputBox;
		std::string m_HoveredItemPath;
		uint32_t m_DisplayedItemCount;

		//Internal
		mutable unsigned int m_ContextMenuID;
		mutable DragDropPayload m_DragDropPayload;
		float m_BottomOffset = 0.0f;
		FileDialogType m_DialogType;
		FileDialogOperation m_DialogOperation;
		FileDialogFilter m_DialogFilter;
		std::vector<FileDialogItem> m_Filter;
		glm::vec2 m_ItemSize;
		ImGuiTextFilter m_SearchFilter;
		
		//Callbacks
		std::function<void(const std::string&)> m_CallbackOnItemClicked;
		std::function<void(const std::string&)> m_CallbackOnItemDoubleClicked;
	};
	
}
