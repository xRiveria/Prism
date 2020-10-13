#include "Prism.h"

class ExampleLayer : public Prism::Layer
{
public:
	ExampleLayer():Layer("Example Layer")
	{

	}

	void OnUpdate() override
	{
		//PRISM_CLIENT_INFO("ExampleLayer::Update");
	}

	void OnEvent(Prism::Event& event) override
	{
		if (event.GetEventType() == Prism::EventType::KeyPressed)
		{
			Prism::KeyPressedEvent& eventReceived = (Prism::KeyPressedEvent&)event;
			if (eventReceived.GetKeyCode() == PRISM_KEY_TAB)
			{
				PRISM_CLIENT_TRACE("Tab was pressed!");
			}
			PRISM_CLIENT_TRACE("{0}", (char)eventReceived.GetKeyCode());
		}
		//PRISM_ENGINE_INFO("{0}", event);
	}
};

class Sandbox : public Prism::Application
{
public:
	Sandbox()
	{
		PRISM_CLIENT_WARN("Created Sandbox Application");
		PushLayer(new ExampleLayer());
		PushOverlay(new Prism::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Prism::Application* Prism::CreateApplication()
{
	return new Sandbox();
}



