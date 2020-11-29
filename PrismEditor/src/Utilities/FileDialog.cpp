#include "FileDialog.h"
#include "imgui/imgui_stdlib.h"

namespace Prism
{

#define OPERATION_NAME (m_DialogOperation == FileDialogOperation_Open) ? "Open" : (m_DialogOperation == FileDialogOperation_Load) ? "Load" : (m_DialogOperation == FileDialogOperation_Save) ? "Save" : "View"
#define FILTER_NAME    (m_DialogFilter == FileDialogFilter_All) ? "All (*.*)"  : (m_DialogFilter == FileDialogFilter_Model) ? "Model(*.*)" : "World (*.world)"

	FileDialog::FileDialog(bool standaloneWindow, FileDialogType dialogType, FileDialogOperation dialogOperation, FileDialogFilter dialogFilter)
	{
		//Context
		m_DialogType = dialogType;
		m_DialogOperation = dialogOperation;
		m_DialogFilter = dialogFilter;
		m_Title = OPERATION_NAME;
		m_IsWindow = standaloneWindow;
		m_ItemSize = ImVec2(100.0f, 100.0f);
		m_IsDirty = true;
		m_SelectionMade = false;
		m_CallbackOnItemClicked = nullptr;
		m_CallbackOnItemDoubleClicked = nullptr;

		//Navigate Call
		m_Navigation.Navigate(WindowsFileSystem::GetProjectDirectory());
	}

	void FileDialog::SetOperation(const FileDialogOperation& operation)
	{
		m_DialogOperation = operation;
		m_Title = OPERATION_NAME;
	}

	//Shows the dialog and returns true if a selection was made.
	bool FileDialog::Show(bool* isVisible, std::string* directory /*= nullptr*/, std::string* filePath /*= nullptr*/)
	{
		if (!(*isVisible))
		{
			m_IsDirty = true; //Set as dirty as things can change until the next time.
			return false;
		}

		m_SelectionMade = false;
		m_IsHoveringItem = false;
		m_IsHoveringWindow = false;

		RenderTopPortion(isVisible); //Top Menu
		RenderMiddlePortion(); //Contents of the current directory.
		RenderBottomPortion(isVisible); //Bottom Menu

		if (m_IsWindow)
		{
			ImGui::End();
		}

		if (m_IsDirty)
		{
			DialogUpdateFromDirectory(m_Navigation.m_CurrentPath);
			m_IsDirty = false;
		}

		if (m_SelectionMade)
		{
			if (directory)
			{
				(*directory) = m_Navigation.m_CurrentPath;
			}

			if (filePath)
			{
				(*filePath) = m_Navigation.m_CurrentPath + "/" + std::string(m_InputBox);
			}
		}

		EmptyAreaContextMenu();

		return m_SelectionMade;
	}

	void FileDialog::RenderTopPortion(bool* isVisible)
	{
		if (m_IsWindow)
		{
			ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver); //No persistent save data.
			ImGui::SetNextWindowSizeConstraints(ImVec2(350, 250), ImVec2(FLT_MAX, FLT_MAX));
			ImGui::Begin(m_Title.c_str(), isVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoDocking);
			ImGui::SetWindowFocus();
		}

		//Directory Navigation Buttons
		{
			//Backwards
			if (ImGui::Button("<"))
			{
				m_IsDirty = m_Navigation.BackwardDirectory();
			}

			ImGui::SameLine();

			//Forwards
			if (ImGui::Button(">"))
			{
				m_IsDirty = m_Navigation.ForwardDirectory();
			}

			//Individual Directories Button
			for (uint32_t i = 0; i < m_Navigation.m_PathHierarchy.size(); i++)
			{
				ImGui::SameLine();
				if (ImGui::Button(m_Navigation.m_PathHierarchyLabels[i].c_str()))
				{
					m_IsDirty = m_Navigation.Navigate(m_Navigation.m_PathHierarchy[i]);
				}
			}
		}

		//Size Slider
		const float sliderWidth = 200.0f;
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - sliderWidth);
		ImGui::PushItemWidth(sliderWidth);
		
		const float previousWidth = m_ItemSize.x;
		ImGui::SliderFloat("##FileDialogSlider", &m_ItemSize.x, m_MinimumItemSize, m_MaximumItemSize); //Use ## to ignore text after the aforementioned tokens.
		m_ItemSize.y += m_ItemSize.x - previousWidth;
		ImGui::PopItemWidth();

		//Search Filter
		const float labelWidth = 37.0f;
		m_SearchFilter.Draw("Filter", ImGui::GetContentRegionAvail().x - labelWidth);

		ImGui::Separator();
	}

	void FileDialog::RenderMiddlePortion()
	{
		//Compute Some Useful Stuff
		const ImGuiWindow* window = ImGui::GetCurrentWindowRead();
		const float contentWidth = ImGui::GetContentRegionAvail().x;
		const float contentHeight = ImGui::GetContentRegionAvail().y - m_BottomOffset;
		
		ImGuiContext& context = *GImGui;
		ImGuiStyle& style = ImGui::GetStyle();

		const float fontHeight = context.FontSize;
		const float labelHeight = fontHeight;
		const float textOffset = 3.0f;
		float penXMinimum = 0.0f;
		float penX = 0.0f;
		bool newLine = true;
		m_DisplayedItemCount = 0;

		ImRect rectButton;
		ImRect rectLabel;

		//Remove Border
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);

		//Make background slightly darker.
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(
			static_cast<int>(m_ContentBackground.x),
			static_cast<int>(m_ContentBackground.y),
			static_cast<int>(m_ContentBackground.z),
			static_cast<int>(m_ContentBackground.w)));

		if (ImGui::BeginChild("##ContentRegion", ImVec2(contentWidth, contentHeight), true))
		{
			m_IsHoveringWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) ? true : m_IsHoveringWindow;

			//Set Starting Position
			{
				float offset = ImGui::GetStyle().ItemSpacing.x;
				penXMinimum = ImGui::GetCursorPosX() + offset;
				ImGui::SetCursorPosX(penXMinimum);
			}

			//Go through all the items.
			for (int i = 0; i < m_Items.size(); i++)
			{
				//Get the item to be displayed.
				FileDialogItem& item = m_Items[i];

				//Apply search filter.
				if (!m_SearchFilter.PassFilter(item.GetLabel().c_str()))
				{
					continue;
				}

				m_DisplayedItemCount++;

				//Start new line?
				if (newLine)
				{
					ImGui::BeginGroup();
					newLine = false;
				}

				ImGui::BeginGroup(); //The look of each file.
				{
					//Compute rectanges for elements that make up an item.
					{
						rectButton = ImRect(
							ImGui::GetCursorScreenPos().x,
							ImGui::GetCursorScreenPos().y,
							ImGui::GetCursorScreenPos().x + m_ItemSize.x,
							ImGui::GetCursorScreenPos().y + m_ItemSize.y
						);

						rectLabel = ImRect(
							rectButton.Min.x,
							rectButton.Max.y - labelHeight - style.FramePadding.y,
							rectButton.Max.x,
							rectButton.Max.y
						);
					}

					//Drop shadow effect.
					if (m_DropShadow)
					{
						static const float shadowThickness = 2.0f;
						ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_BorderShadow];
						ImGui::GetWindowDrawList()->AddRectFilled(rectButton.Min, ImVec2(rectLabel.Max.x + shadowThickness, rectLabel.Max.y + shadowThickness), IM_COL32(color.x * 255, color.y * 255, color.z * 255, color.w * 255));
					}

					//Thumbnail
					{
						ImGui::PushID(i);
						ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.25f));

						if (ImGui::Button("##Dummy", m_ItemSize))
						{
							//Determine type of click.
							item.Clicked();
							const bool IsSingleClick = item.GetTimeSinceLastClickMilliseconds() > 500;

							if (IsSingleClick) //Do not navigate, just display information.
							{
								//Updated Input Box
								m_InputBox = item.GetLabel();

								//Callback
								if (m_CallbackOnItemClicked)
								{
									m_CallbackOnItemClicked(item.GetPath());
								}
							}
							else //Is Double Click
							{
								m_IsDirty = m_Navigation.Navigate(item.GetPath());
								m_SelectionMade = !item.IsDirectory();

								//When browsing files, open them on double click.
								if (m_DialogType == FileDialogType_Browser)
								{
									if (!item.IsDirectory())
									{
										WindowsFileSystem::OpenDirectoryWindow(item.GetPath());
									}
								}

								//Callback
								if (m_CallbackOnItemDoubleClicked)
								{
									m_CallbackOnItemDoubleClicked(m_Navigation.m_CurrentPath);
								}
							}
						}

						//Item Functionality
						{
							//Manually detect some useful states?
							if (ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly))
							{
								m_IsHoveringItem = true;
								m_HoveredItemPath = item.GetPath();
							}

							ItemClick(&item);
							ItemContextMenu(&item);
							ItemDrag(&item);
						}

						ImGui::SetCursorScreenPos(ImVec2(rectButton.Min.x + style.FramePadding.x, rectButton.Min.y + style.FramePadding.y));

						ImGui::Image((void*)item.GetTexture()->GetTextureID(), 
							ImVec2(rectButton.Max.x - rectButton.Min.x - style.FramePadding.x * 2.0f,
							rectButton.Max.y - rectButton.Min.y - style.FramePadding.y - labelHeight - 5.0f));

						ImGui::PopStyleColor(2);
						ImGui::PopID();
					}

					//Label
					{
						const char* labelText = item.GetLabel().c_str();
						const ImVec2 labelSize = ImGui::CalcTextSize(labelText, nullptr, true);

						//Draw Text Background
						ImGui::GetWindowDrawList()->AddRectFilled(rectLabel.Min, rectLabel.Max, IM_COL32(51, 51, 51, 190));

						//Draw Text
						ImGui::SetCursorScreenPos(ImVec2(rectLabel.Min.x + textOffset, rectLabel.Min.y + textOffset));
						if (labelSize.x <= m_ItemSize.x && labelSize.y <= m_ItemSize.y)
						{
							ImGui::TextUnformatted(labelText);
						}
						else
						{
							ImGui::RenderTextClipped(rectLabel.Min, rectLabel.Max, labelText, nullptr, &labelSize, ImVec2(0, 0), &rectLabel);
						}
					}

					ImGui::EndGroup();
				}

				//Decide whether we shouls switch to the next column or switch rows.
				penX += m_ItemSize.x + ImGui::GetStyle().ItemSpacing.x;
				if (penX >= contentWidth - m_ItemSize.x)
				{
					ImGui::EndGroup();
					penX = penXMinimum;
					ImGui::SetCursorPosX(penX);
					newLine = true;
				}
				else
				{
					ImGui::SameLine();
				}
			}

			if (!newLine)
			{
				ImGui::EndGroup();
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

	void FileDialog::RenderBottomPortion(bool* isVisible)
	{
		if (m_DialogType == FileDialogType_Browser)
		{
			//Move to the bottom of the window.
			m_BottomOffset = 20.0f;
			ImGui::SetCursorPosY(ImGui::GetWindowSize().y - m_BottomOffset);

			const char* text = (m_DisplayedItemCount == 1) ? "%d Item" : "%d Items"; //If there's only one item, have no 's', else we use it.
			ImGui::Text(text, m_DisplayedItemCount);
		}
		else
		{
			//Move to the bottom of the window.

			m_BottomOffset = 35.0f;
			ImGui::SetCursorPosY(ImGui::GetWindowSize().y - m_BottomOffset);

			ImGui::PushItemWidth(ImGui::GetWindowSize().x - 235);
			ImGui::InputText("##InputBox", &m_InputBox);
			ImGui::PopItemWidth();

			ImGui::SameLine();

			ImGui::Text(FILTER_NAME);
			ImGui::SameLine();

			if (ImGui::Button(OPERATION_NAME))
			{
				m_SelectionMade = true;
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				m_SelectionMade = false;
				(*isVisible) = false;
			}
		}
	}

	void FileDialog::ItemDrag(FileDialogItem* item) const
	{
		//To Be Implemented.
	}

	void FileDialog::ItemClick(FileDialogItem* item) const
	{
		//To Be Implemented.
	}

	void FileDialog::ItemContextMenu(FileDialogItem* item) const
	{
		//To Be Implemented.
	}

	bool FileDialog::DialogUpdateFromDirectory(const std::string& path)  
	{
		if (!WindowsFileSystem::IsDirectoryValid(path))
		{
			PRISM_CLIENT_ASSERT(false, "Directory Invalid!");
			return false;
		}

		m_Items.clear();
		m_Items.shrink_to_fit();

		//Get Directories
		std::vector<std::string> childDirectories = WindowsFileSystem::GetDirectoriesInDirectory(path);
		for (const auto& childDirectory : childDirectories)
		{
			m_Items.emplace_back(childDirectory, GetIconTexture());
		}

		//Get Files (Based on Filter)
		std::vector<std::string> childFiles;
		if (m_DialogFilter == FileDialogFilter_All)
		{
			childFiles = WindowsFileSystem::GetFilesInDirectory(path);
			for (const auto& childFile : childFiles)
			{
				//Add filters here.
				m_Items.emplace_back(childFile, GetIconTexture());
			}
		}

		else if (m_DialogFilter == FileDialogFilter_Scene)
		{
			childFiles = WindowsFileSystem::GetSupportedSceneFilesInDirectory(path);
			for (const auto& childFile : childFiles)
			{
				m_Items.emplace_back(childFile, GetIconTexture());
			}
		}
		
		else if (m_DialogFilter == FileDialogFilter_Model)
		{
			childFiles = WindowsFileSystem::GetSupportedModelFilesInDirectory(path);
			for (const auto& childFile : childFiles)
			{
				m_Items.emplace_back(childFile, GetIconTexture());
			}
		}

		return true;
	}

	void FileDialog::EmptyAreaContextMenu()
	{
		//To Be Implemented.
	}

}