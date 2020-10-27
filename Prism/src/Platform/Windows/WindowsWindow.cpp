#include "PrismPrecompiledHeader.h"
#include "WindowsWindow.h"
#include "Prism/Events/ApplicationEvent.h"
#include "Prism/Events/KeyEvent.h"
#include "Prism/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

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
		PRISM_PROFILE_FUNCTION();

		Initialize(windowProperties);
	}

	WindowsWindow::~WindowsWindow()
	{
		PRISM_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Initialize(const WindowProperties& windowProperties)
	{
		PRISM_PROFILE_FUNCTION();

		m_Data.windowTitle = windowProperties.windowTitle;
		m_Data.windowWidth = windowProperties.windowWidth;
		m_Data.windowHeight = windowProperties.windowHeight;

		PRISM_ENGINE_INFO("Creating a {0}x{1} Window for {2}.", windowProperties.windowWidth, windowProperties.windowHeight, windowProperties.windowTitle);

		//We might want to create multiple windows, but we only want to initialize GLFW once, hence the static boolean. 
		if (!s_IsGLFWInitialized)
		{
			PRISM_PROFILE_SCOPE("glfwInit");

			int glfwInitializationStatus = glfwInit();
			PRISM_ENGINE_ASSERT(glfwInitializationStatus, "Failed to Initialize GLFW.");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_IsGLFWInitialized = true;
		}

		{
			PRISM_PROFILE_SCOPE("glfwCreateWindow");

			m_Window = glfwCreateWindow((int)windowProperties.windowWidth, (int)windowProperties.windowHeight, windowProperties.windowTitle.c_str(), nullptr, nullptr);
		}

		m_GraphicsContext = new OpenGLContext(m_Window);
		m_GraphicsContext->Initialize();
	
		glfwSetWindowUserPointer(m_Window, &m_Data); //Our defined pointer for events. 
		SetVSync(true);

		//Setup GLFW Event Callbacks

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int windowWidth, int windowHeight)
		{
			WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);		
			windowData.windowWidth = windowWidth;
			windowData.windowHeight = windowHeight;

			WindowResizeEvent resizeEvent(windowWidth, windowHeight);
			PRISM_ENGINE_WARN("{0} & {1}", windowWidth, windowHeight);
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
		PRISM_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		PRISM_PROFILE_FUNCTION();

		glfwPollEvents();
		m_GraphicsContext->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		PRISM_PROFILE_FUNCTION();

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
