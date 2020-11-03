#include "PrismPrecompiledHeader.h"
#include "Application.h"
#include "Platform/Windows/WindowsInput.h"
#include "Prism/ImGui/ImGuiLayer.h"
#include "GLFW/glfw3.h"
#include "Prism/Renderer/Renderer.h"

namespace Prism
{
	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		PRISM_PROFILE_FUNCTION();

		PRISM_ENGINE_ASSERT(!s_Instance, "Application already exists.");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::ConstructWindow()); //Explicit conversion here that converts the created Window's pointer from ConstructWindow() into a unique pointer that is returned here.
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));		
		m_Window->SetVSync(true); //Lock monitor refresh rate.

		Renderer::InitializeRenderer();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::PushLayer(Layer* layer)
	{
		PRISM_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		PRISM_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::CloseApplication()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& event)
	{
		PRISM_PROFILE_FUNCTION();

		EventDispatcher eventDispatcher(event);
		//If the event that comes through the dispatcher is a WindowCloseEvent, we will call the binded function.
		eventDispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		eventDispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));


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
		PRISM_PROFILE_FUNCTION();
		while (m_Running)
		{		
			PRISM_PROFILE_SCOPE("Run Loop");

			//Renderer::FlushRenderer();
			float time = (float)glfwGetTime();  //Platform::GetTime() in the future. Gets the current time.
			Timestep timestep = time - m_LastFrameTime; //The time it took from the previous frame to the current frame. 
			m_LastFrameTime = time; //Sets the previous frame's time to the current time. Repeat.

			if (m_Minimized != true)
			{
				PRISM_PROFILE_SCOPE("LayerStack Updates");
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}
			
			//Don't include ImGui inside the minimize loop as certain ImGui stuff might be undocked that you may still want to receive updates.
			m_ImGuiLayer->BeginImGuiRenderLoop();
			
			{
				PRISM_PROFILE_SCOPE("LayerStack OnImGuiRender");
				for (Layer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}

				m_ImGuiLayer->EndImGuiRenderLoop();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& closeEvent)
	{
		m_Running = false;
		return true; //Handled.
	}

	bool Application::OnWindowResize(WindowResizeEvent& resizeEvent)
	{
		PRISM_PROFILE_FUNCTION();
		//Stop rendering if window is minimized. 
		if (resizeEvent.GetWidth() == 0 || resizeEvent.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(resizeEvent.GetWidth(), resizeEvent.GetHeight());

		return false;
	}
}

 