#pragma once
#include "Core.h"
#include "Windows.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Prism/LayerStack.h"
#include "Prism/ImGui/ImGuiLayer.h"

//This class is meant to be inherited by all Prism applications.
//In this case, our Sandbox application is the first to do this.
namespace Prism
{
	class PRISM_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() const { return *m_Window; }
		inline static Application& GetApplication() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& closeEvent);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;
		unsigned int m_VertexBuffer;
		unsigned int m_IndexBuffer;

	private:
		static Application* s_Instance; 
	};

	//To be defined in the Client application.
	//This is an application singleton that specifies there will only be one application for the application's lifetime. 
	Application* CreateApplication();
}

