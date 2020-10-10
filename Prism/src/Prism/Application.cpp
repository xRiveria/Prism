#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace Prism
{
	Application::Application()
	{
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 1080);
		PRISM_CLIENT_TRACE(e);
		while (true);
	}
}
