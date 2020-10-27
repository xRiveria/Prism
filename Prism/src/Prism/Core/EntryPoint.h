#pragma once

#ifdef PRISM_PLATFORM_WINDOWS
extern Prism::Application* Prism::CreateApplication();

//Creates the actual application for us and setups all related system.
 
int main(int argc, char** argv)
{
	Prism::Log::Initialize();
	PRISM_ENGINE_INFO("Initialized Prism Logging Library");
	PRISM_CLIENT_INFO("Initalized Application Logging Library");

	PRISM_PROFILE_BEGIN_SESSION("Startup", "PrismProfile-Startup.json");
	Prism::Application* app = Prism::CreateApplication();
	PRISM_PROFILE_END_SESSION();

	PRISM_PROFILE_BEGIN_SESSION("Runtime", "PrismProfile-Runtime.json");
	app->Run();
	PRISM_PROFILE_END_SESSION();

	PRISM_PROFILE_BEGIN_SESSION("Shutdown", "PrismProfile-Shutdown.json");
	delete app;	
	PRISM_PROFILE_END_SESSION();
}
#endif
