#include "FileDialog.h"

namespace Prism
{

#define OPERATION_NAME (m_DialogOperation == FileDialogOperation_Open) ? "Open" : (m_DialogOperation == FileDialogOperation_Load) ? "Load" : (m_DialogOperation == FileDialogOperation_Save) ? "Save" : "View"
#define FILTER_NAME


	FileDialog::FileDialog(bool standaloneWindow, FileDialogType dialogType, FileDialogOperation dialogOperation, FileDialogFilter dialogFilter)
	{
		//Context
		m_DialogType = dialogType;
		m_DialogOperation = dialogOperation;
		m_DialogFilter = dialogFilter;
		m_Title = OPERATION_NAME;
		m_IsWindow = standaloneWindow;
		m_ItemSize = glm::vec2(100.0f, 100.0f);
		m_IsDirty = true;
		m_SelectionMade = false;
		m_CallbackOnItemClicked = nullptr;
		m_CallbackOnItemDoubleClicked = nullptr;
		//Navigate Call
	}

	void FileDialog::SetOperation(const FileDialogOperation& operation)
	{
		m_DialogOperation = operation;
		m_Title = OPERATION_NAME;
	}

	//Shows the dialog and returns true if a selection was made.
	bool FileDialog::Show(bool* isVisible, std::string* directory /*= nullptr*/, std::string* filePath /*= nullptr*/)
	{
		if (!(isVisible))
		{
			m_IsDirty = true; //Set as dirty as things can change until the next time.
			return false;
		}

		m_SelectionMade = false;
		m_IsHoveringItem = false;
		m_IsHoveringWindow = false;

		ShowTop(isVisible); //Top Menu
		ShowMiddle(); //Contents of the current directory.
		ShowBottom(isVisible); //Bottom Menu

		if (m_IsWindow)
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

	void FileDialog::ShowTop(bool* isVisible)
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
		ImGui::SliderFloat("FileDialogSlider", &m_ItemSize.x, m_MinimumItemSize, m_MaximumItemSize);
		m_ItemSize.y += m_ItemSize.x - previousWidth;
		ImGui::PopItemWidth();

		//Search Filter
		const float labelWidth = 37.0f;
		m_SearchFilter.Draw("Filter", ImGui::GetContentRegionAvail().x - labelWidth);

		ImGui::Separator();
	}

	void FileDialog::ShowMiddle()
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
		const float penXMinimum = 0.0f;
		const float penX = 0.0f;
		bool newLine = true;
		m_DisplayedItemCount = 0;

		ImRect rectButton;
		ImRect rectLabel;

	}

	void FileDialog::ShowBottom(bool* isVisible)
	{
	}

}