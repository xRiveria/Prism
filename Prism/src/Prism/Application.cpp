#include "PrismPrecompiledHeader.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include <GLFW/glfw3.h>

namespace Prism
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::ConstructWindow()); //Explicit conversion here that converts the created Window's pointer from ConstructWindow() into a unique pointer that is returned here.
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}
