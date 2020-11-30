#include "ConsoleWidget.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_internal.h"

namespace Prism
{
	template <class T>
	constexpr T Max(T a, T b) { return a > b ? a : b; }

	ConsoleWidget::ConsoleWidget()
	{
		m_EditorLogger = CreateReference<EditorLogger>();
		m_EditorLogger->SetCallback([this](const LogPackage& package)
		{
			AddLogPackage(package);
		});

		Log::SetEditorLogger(m_EditorLogger);
		PRISM_EDITOR_INFO("Initialized Editor Logger");
		PRISM_EDITOR_WARN("Test Warning");
		PRISM_EDITOR_ERROR("Test Error");
	}

	bool ConsoleWidget::CreateImGuiImageButton(const IconType& icon, const float& size)
	{
		return ImGui::ImageButton((void*)IconProvider::GetInstance().GetTextureByType(icon)->GetTextureID(), ImVec2(size, size), ImVec2(0, 1),ImVec2(1, 0));
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

		const auto ButtonLogTypeVisibilityToggle = [this](const IconType& icon, const uint32_t& index)
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
		ButtonLogTypeVisibilityToggle(IconType::Icon_Console_Info, 0);
		ButtonLogTypeVisibilityToggle(IconType::Icon_Console_Warning, 1);
		ButtonLogTypeVisibilityToggle(IconType::Icon_Console_Error, 2);

		//Text Filter
		const float label_width = 150.0f;
		m_LogTextFilter.Draw("Filter", ImGui::GetContentRegionAvail().x - label_width);
		ImGui::Separator();

		//Content
		if (ImGui::BeginChild("Scrolling"), ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)
		{
			ImVec4 oddColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
			ImVec4 evenColor = oddColor;
			evenColor.w = 0;

			//Set Max Log Width
			float maxLogWidth = 0;
			maxLogWidth = m_LogTypeVisibility[0] ? Max(maxLogWidth, m_LogTypeMaxWidth[0]) : maxLogWidth;
			maxLogWidth = m_LogTypeVisibility[0] ? Max(maxLogWidth, m_LogTypeMaxWidth[1]) : maxLogWidth;
			maxLogWidth = m_LogTypeVisibility[0] ? Max(maxLogWidth, m_LogTypeMaxWidth[2]) : maxLogWidth;
			maxLogWidth = Max(maxLogWidth, ImGui::GetWindowContentRegionWidth());
			ImGui::PushItemWidth(maxLogWidth);

			uint32_t index = 0;
			for (LogPackage& log : m_Logs)
			{
				if (m_LogTextFilter.PassFilter(log.m_LogText.c_str()))
				{
					if (m_LogTypeVisibility[log.m_ErrorLevel])
					{
						//LogEntry
						ImGui::BeginGroup();
						{
							ImGui::PushStyleColor(ImGuiCol_Text, m_LogTypeColor[log.m_ErrorLevel]); //Text Color
							ImGui::PushStyleColor(ImGuiCol_FrameBg, index % 2 != 0 ? oddColor : evenColor);
							ImGui::InputText("##log", &log.m_LogText, ImGuiInputTextFlags_ReadOnly);
							ImGui::PopStyleColor(2);

							ImGui::EndGroup();
						}
					}
					index++;
				}
			}
		
			ImGui::PopItemWidth();

			ImGui::EndChild();
		}

		ImGui::End();
	}

	void ConsoleWidget::ClearAllLogs()
	{
		m_Logs.clear();
		m_Logs.shrink_to_fit();

		m_LogTypeMaxWidth[0] = 0;
		m_LogTypeMaxWidth[1] = 0;
		m_LogTypeMaxWidth[2] = 0;

		m_LogTypeCount[0] = 0;
		m_LogTypeCount[1] = 0;
		m_LogTypeCount[2] = 0;
	}

	void ConsoleWidget::AddLogPackage(const LogPackage& package)
	{
		//Save to Deque
		m_Logs.push_back(package);
		if (static_cast<uint32_t>(m_Logs.size()) > m_MaximumLogCount)
		{
			m_Logs.pop_front();
		}

		//Update Count
		m_LogTypeCount[package.m_ErrorLevel]++;

		// Compute max width
		float& width = m_LogTypeMaxWidth[package.m_ErrorLevel];
		if (ImGui::GetCurrentContext()->Font)
		{
			width = Max(width, ImGui::CalcTextSize(package.m_LogText.c_str()).x + 10);
		}
		else
		{
			// During startup, the font can be null, so compute a poor man's width
			width = Max(width, package.m_LogText.size() * 23.0f);
		}

		// If the user is displaying this type of messages, scroll to bottom
		if (m_LogTypeVisibility[package.m_ErrorLevel])
		{
			//m_scroll_to_bottom = true;
		}

	}
}