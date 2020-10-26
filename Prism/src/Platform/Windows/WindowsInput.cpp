#include "PrismPrecompiledHeader.h"
#include "WindowsInput.h"
#include "GLFW/glfw3.h"
#include "Prism/Core/Application.h"

namespace Prism
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImplementation(int keyCode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		auto keyState = glfwGetKey(window, keyCode);
		return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImplementation(int mouseButton)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		auto mouseState = glfwGetMouseButton(window, mouseButton);
		return mouseState == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImplementation()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double xPosition, yPosition;
		glfwGetCursorPos(window, &xPosition, &yPosition);

		return { (float)xPosition, (float)yPosition };
	}

	float WindowsInput::GetMouseXImplementation()
	{
		auto [x, y] = GetMousePositionImplementation();
		return x;
	}

	float WindowsInput::GetMouseYImplementation()
	{
		auto [x, y] = GetMousePositionImplementation();
		return y;
	}
}