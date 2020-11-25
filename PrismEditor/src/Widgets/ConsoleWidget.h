#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <deque>
#include <atomic>
#include <imgui/imgui.h>
#include <stb_image/stb_image.h>
#include "Prism.h"

namespace Prism
{
	struct LogPackage
	{
		LogPackage(const std::string& text, const int errorLevel) : m_LogText(text), m_ErrorLevel(errorLevel) {}

		std::string m_LogText;
		unsigned int m_ErrorLevel = 0;
	};
	
	//To Be Moved
	enum Icon_Type
	{
		Icon_Console_Info,
		Icon_Console_Warning,
		Icon_Console_Error
	};

	class ConsoleWidget
	{
	public:
		ConsoleWidget();
		bool CreateImGuiImageButton(const Icon_Type& icon, const float& size);
		void OnConsoleWidgetUpdate();
		void AddLogPackage(const LogPackage& package);
		void ClearAllLogs();

	private:
		bool m_ScrollToBottom = false;
		uint32_t m_MaximumLogCount = 1000;

		//Individual Log Types
		float m_LogTypeMaxWidth[3] = { 0, 0, 0 };
		bool m_LogTypeVisibility[3] = { true, true, true };
		uint32_t m_LogTypeCount[3] = { 0, 0, 0 };
		const std::vector<ImVec4> m_LogTypeColor =
		{
			ImVec4(0.76f, 0.77f, 0.80f, 1.0f),  //Info
			ImVec4(0.70f, 0.75f, 0.20f, 1.0f),   //Warning
			ImVec4(0.70f, 0.30f, 0.30f, 1.0f)   //Error
		};

		//Icon Types
		Reference<Texture2D> m_LogInfoIcon;
		Reference<Texture2D> m_LogWarningIcon;
		Reference<Texture2D> m_LogErrorIcon;

		//std::atomic<bool> m_IsReading = false;
		ImGuiTextFilter m_LogTextFilter;
		std::deque<LogPackage> m_Logs;
	};
}