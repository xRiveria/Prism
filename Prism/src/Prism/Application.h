#pragma once
#include "Core.h"

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
	};

	//To be defined in the Client application.
	Application* CreateApplication();
}

