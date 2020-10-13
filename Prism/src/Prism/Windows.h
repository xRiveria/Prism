#pragma once

#include "PrismPrecompiledHeader.h"
#include "Prism/Core.h"
#include "Prism/Events/Event.h"

namespace Prism
{
	//Abstract representation of a standard Window.
	struct WindowProperties
	{
		std::string windowTitle;
		unsigned int windowWidth;
		unsigned int windowHeight;

		WindowProperties(const std::string& title = "Prism Engine", unsigned int width = 1280, unsigned int height = 720)
			: windowTitle(title), windowWidth(width), windowHeight(height)
		{}
	};

	//Interface representing a desktop (Windows, Mac etc) system based window.
	//This is a pure virtual class no data nor functions.
	//It is meant to be implemented per platform.
	class PRISM_API Window
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;
		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		
		virtual inline unsigned int GetWindowWidth() const = 0;
		virtual inline unsigned int GetWindowHeight() const = 0;
		virtual inline void* GetConstructedWindow() const = 0;

		//Window Attributes
		virtual inline void SetEventCallback(const EventCallbackFunction& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSyncActive() const = 0;

		//Because this is platform independant, this ConstructWindow function has to be created per platform.
		//We leave the definition of ConstructWindow to whichever platform inherits and implements this in their .CPP file. 
		//Note that this only constructs the Window's properties, not the actual window itself unless defined.
		static Window* ConstructWindow(const WindowProperties& windowProperties = WindowProperties());
	};
}