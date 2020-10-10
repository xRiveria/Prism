#pragma once

#ifdef PRISM_PLATFORM_WINDOWS
extern Prism::Application* Prism::CreateApplication();

//Creates the actual application for us.

int main(int argc, char** argv)
{
	std::cout << "Prism Engine\n";
	auto app = Prism::CreateApplication();
	app->Run();
	delete app;
}
#endif
