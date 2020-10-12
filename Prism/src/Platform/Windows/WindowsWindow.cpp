#include "PrismPrecompiledHeader.h"
#include "WindowsWindow.h"
#include "Prism/Events/ApplicationEvent.h"
#include "Prism/Events/KeyEvent.h"
#include "Prism/Events/MouseEvent.h"
#include "glad/glad.h"

namespace Prism
{
	static bool s_IsGLFWInitialized = false;  

	static void GLFWErrorCallback(int error, const char* errorDescription)
	{
		PRISM_ENGINE_ERROR("GLFW Error ({0}): {1}", error, errorDescription);
	}

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
			int glfwInitializationStatus = glfwInit();
			PRISM_ENGINE_ASSERT(glfwInitializationStatus, "Failed to Initialize GLFW.");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_IsGLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)windowProperties.windowWidth, (int)windowProperties.windowHeight, windowProperties.windowTitle.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		
		int gladInitializationStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //Returns the current OpenGL context.
		PRISM_ENGINE_ASSERT(gladInitializationStatus, "Failed to Initialize Glad.");

		glfwSetWindowUserPointer(m_Window, &m_Data); //Our defined pointer for events. 
		SetVSync(true);

		//Setup GLFW Event Callbacks

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int windowWidth, int windowHeight)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);		
			windowData.windowWidth = windowWidth;
			windowData.windowHeight = windowHeight;

			WindowResizeEvent resizeEvent(windowWidth, windowHeight);
			windowData.EventCallback(resizeEvent);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent closeEvent;
			windowData.EventCallback(closeEvent);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int keyPressed, int scanCode, int action, int mods)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent keyPressedEvent(keyPressed, 0);
					windowData.EventCallback(keyPressedEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent keyReleasedEvent(keyPressed);
					windowData.EventCallback(keyReleasedEvent);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent keyRepeatEvent(keyPressed, 1);
					windowData.EventCallback(keyRepeatEvent);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent keyTypedEvent(character);
			windowData.EventCallback(keyTypedEvent);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int buttonPressed, int action, int mods)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent mousePressedEvent(buttonPressed);
					windowData.EventCallback(mousePressedEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent mouseReleasedEvent(buttonPressed);
					windowData.EventCallback(mouseReleasedEvent);
					break;
				}
			}		
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent scrollEvent((float)xOffset, (float)yOffset);
			windowData.EventCallback(scrollEvent);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPosition, double yPosition)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent moveEvent((float)xPosition, (float)yPosition);
			windowData.EventCallback(moveEvent);
		});
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
