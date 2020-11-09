#pragma once

#include "PrismPrecompiledHeader.h"
#include "Prism/Core/Core.h"
#include "Prism/Events/Event.h"

namespace Prism
{
	//Abstract representation of a standard Window.
	struct WindowProperties
	{
		std::string windowTitle;
		uint32_t windowWidth;
		uint32_t windowHeight;

		WindowProperties(const std::string& title = "Prism Engine", uint32_t width = 1600, uint32_t height = 900)
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
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;
		
		virtual inline uint32_t GetWindowWidth() const = 0;
		virtual inline uint32_t GetWindowHeight() const = 0;
		inline virtual void* GetNativeWindow() const = 0;

		//Window Attributes
		virtual inline void SetEventCallback(const EventCallbackFunction& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetWindowIcon(const std::string& filePath) const = 0;
		virtual bool IsVSyncActive() const = 0;

		//Because this is platform independant, this ConstructWindow function has to be created per platform.
		//We leave the definition of ConstructWindow to whichever platform inherits and implements this in their .CPP file. 
		//Note that this only constructs the Window's properties, not the actual window itself unless defined.
		static Window* ConstructWindow(const WindowProperties& windowProperties = WindowProperties());
	};
}