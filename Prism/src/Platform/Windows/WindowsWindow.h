#pragma once

#include "Prism/Windows.h"
#include "GLFW/glfw3.h"

namespace Prism
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& windowProperties);
		virtual ~WindowsWindow();

		void OnUpdate() override; 

		inline unsigned int GetWindowWidth() const override { return m_Data.windowWidth; }
		inline unsigned int GetWindowHeight() const override { return m_Data.windowHeight; }
		inline void* GetConstructedWindow() const override { return m_Window; }

		//Window Attributes
		inline void SetEventCallback(const EventCallbackFunction& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSyncActive() const override;

		//Windows Specific
		void SetWindowsIcon(const std::string& iconPath) const;

	private:
		virtual void Initialize(const WindowProperties& windowProperties);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string windowTitle;
			unsigned int windowWidth, windowHeight;
			bool vSync;

			EventCallbackFunction EventCallback;
		};

		WindowData m_Data;
	};
}
