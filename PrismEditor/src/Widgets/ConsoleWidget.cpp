#include "ConsoleWidget.h"

namespace Prism
{
	ConsoleWidget::ConsoleWidget()
	{
		m_LogInfoIcon = Texture2D::CreateTexture("assets/icons/console_info.png");
		m_LogWarningIcon = Texture2D::CreateTexture("assets/icons/console_warning.png");
		m_LogErrorIcon = Texture2D::CreateTexture("assets/icons/console_error.png");
	}

	bool ConsoleWidget::CreateImGuiImageButton(const Icon_Type& icon, const float& size)
	{
		switch (icon)
		{
			case Icon_Type::Icon_Console_Info:
			{
				return ImGui::ImageButton((void*)ConsoleWidget::m_LogInfoIcon->GetTextureID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0));
			};
			case Icon_Type::Icon_Console_Warning:
			{
				return ImGui::ImageButton((void*)ConsoleWidget::m_LogWarningIcon->GetTextureID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0));
			}
			case Icon_Type::Icon_Console_Error:
			{
				return ImGui::ImageButton((void*)ConsoleWidget::m_LogErrorIcon->GetTextureID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0));
			}
		}

		return ImGui::ImageButton((void*)ConsoleWidget::m_LogInfoIcon->GetTextureID(), ImVec2(size, size), ImVec2(0, 1),ImVec2(1, 0));
	}

	void ConsoleWidget::OnConsoleWidgetUpdate()
	{
		ImGui::Begin("Console");

		//Clear Logs Button
		if (ImGui::Button("Clear"))
		{
			ClearAllLogs();
		}
		ImGui::SameLine();

		const auto ButtonLogTypeVisibilityToggle = [this](const Icon_Type& icon, const uint32_t& index)
		{
			bool& visibility = m_LogTypeVisibility[index];
			ImGui::PushStyleColor(ImGuiCol_Button, visibility ? ImGui::GetStyle().Colors[ImGuiCol_Button] : ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
			
			if (CreateImGuiImageButton(icon, 15.0f))
			{
				visibility = !visibility;
				m_ScrollToBottom = true;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::Text("%d", m_LogTypeCount[index]);
			ImGui::SameLine();
		};

		//Log Type Visibility Buttons
		ButtonLogTypeVisibilityToggle(Icon_Console_Info, 0);
		ButtonLogTypeVisibilityToggle(Icon_Console_Warning, 0);
		ButtonLogTypeVisibilityToggle(Icon_Console_Error, 0);

		//Text Filter
		const float label_width = 150.0f;
		m_LogTextFilter.Draw("Filter", ImGui::GetContentRegionAvail().x - label_width);
		ImGui::Separator();

		ImGui::End();
	}

	void ConsoleWidget::ClearAllLogs()
	{

	}
}