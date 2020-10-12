#include "Prism.h"

class ExampleLayer : public Prism::Layer
{
public:
	ExampleLayer():Layer("Example Layer")
	{
		PRISM_CLIENT_INFO("Created Example Layer");
	}

	void OnUpdate() override
	{
		//PRISM_CLIENT_INFO("ExampleLayer::Update");
	}

	void OnEvent(Prism::Event& event) override
	{
		PRISM_ENGINE_INFO("{0}", event);
	}
};

class Sandbox : public Prism::Application
{
public:
	Sandbox()
	{
		PRISM_CLIENT_WARN("Created Sandbox Application");
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Prism::Application* Prism::CreateApplication()
{
	return new Sandbox();
}



