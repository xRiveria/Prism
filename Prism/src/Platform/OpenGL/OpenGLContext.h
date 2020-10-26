#pragma once
#include "Prism/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

struct GLFWwindow;

namespace Prism 
{
	class OpenGLContext : public GraphicsContext 
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Initialize() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}

