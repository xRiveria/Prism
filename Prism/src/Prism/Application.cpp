#include "PrismPrecompiledHeader.h"
#include "Application.h"
#include <GLFW/glfw3.h>

namespace Prism
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::ConstructWindow()); //Explicit conversion here that converts the created Window's pointer from ConstructWindow() into a unique pointer that is returned here.
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{
		
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher eventDispatcher(event);
		//If the event that comes through the dispatcher is a WindowCloseEvent, we will call the binded function.
		eventDispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.IsEventHandled)
			{
				break;
			}
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& closeEvent)
	{
		m_Running = false;
		return true; //Handled.
	}
}
