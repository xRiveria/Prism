#pragma once

#ifdef PRISM_PLATFORM_WINDOWS
extern Prism::Application* Prism::CreateApplication();

//Creates the actual application for us and setups all related system.
 
int main(int argc, char** argv)
{
	Prism::Log::Initialize();
	PRISM_ENGINE_INFO("Initialized Prism Logging Library");
	PRISM_CLIENT_INFO("Initalized Application Logging Library");

	Prism::Application* app = Prism::CreateApplication();
	app->Run();
	delete app;
}
#endif
