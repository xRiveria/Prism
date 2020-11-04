#pragma once
#include "Core.h"
#include "Windows.h"
#include "Prism/Events/Event.h"
#include "Prism/Events/ApplicationEvent.h"
#include "Prism/Core/LayerStack.h"
#include "Prism/ImGui/ImGuiLayer.h"
#include "Prism/Core/Timestep.h"

//This class is meant to be inherited by all Prism applications.
//In this case, our Sandbox application is the first to do this.
namespace Prism
{
	class PRISM_API Application
	{
	public:
		Application(const std::string& applicationName = "Prism Application");
		virtual ~Application() = default;

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() const { return *m_Window; }
		void CloseApplication();
		inline static Application& GetApplication() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& closeEvent);
		bool OnWindowResize(WindowResizeEvent& resizeEvent);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance; 
	};

	//To be defined in the Client application.
	//This is an application singleton that specifies there will only be one application for the application's lifetime. 
	Application* CreateApplication();
}

