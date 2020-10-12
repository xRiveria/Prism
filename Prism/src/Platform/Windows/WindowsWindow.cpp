#include "PrismPrecompiledHeader.h"
#include "WindowsWindow.h"
#include "Prism/Log.h"
#include "Prism/Core.h"

namespace Prism
{
	static bool s_IsGLFWInitialized = false;  

	Window* Window::ConstructWindow(const WindowProperties& windowProperties)
	{
		return new WindowsWindow(windowProperties); //Constructs window properties, not the actual window. 
	}

	WindowsWindow::WindowsWindow(const WindowProperties& windowProperties)
	{
		Initialize(windowProperties);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Initialize(const WindowProperties& windowProperties)
	{
		m_Data.windowTitle = windowProperties.windowTitle;
		m_Data.windowWidth = windowProperties.windowWidth;
		m_Data.windowHeight = windowProperties.windowHeight;

		PRISM_ENGINE_INFO("Creating a {0}x{1} Window for {2}.", windowProperties.windowWidth, windowProperties.windowHeight, windowProperties.windowTitle);
		
		//We might want to create multiple windows, but we only want to initialize GLFW once, hence the static boolean. 
		if (!s_IsGLFWInitialized)
		{
			int initializationStatus = glfwInit();
			PRISM_ENGINE_ASSERT(initializationStatus, "Could not initialize GLFW.");
			s_IsGLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)windowProperties.windowWidth, (int)windowProperties.windowHeight, windowProperties.windowTitle.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data); //Our defined pointer for events. 
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_Data.vSync = enabled;
	}

	bool WindowsWindow::IsVSyncActive() const
	{
		return m_Data.vSync;
	}

	void WindowsWindow::SetWindowsIcon(const std::string& iconPath) const
	{
		//GLFWimage windowIcon = "Resources/WindowIcon.png";
		glfwSetWindowIcon(m_Window, 0, NULL);
	}

}
