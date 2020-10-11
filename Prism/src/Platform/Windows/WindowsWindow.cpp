#include "PrismPrecompiledHeader.h"
#include "WindowsWindow.h"

namespace Prism
{
	static bool s_IsGLFWInitialized = false;

	Window* Window::ConstructWindow(const WindowProperties& windowProperties)
	{
		return new WindowsWindow(windowProperties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& windowProperties)
	{
		Initialize(windowProperties);
	}

	WindowsWindow::~WindowsWindow()
	{

	}

}
