#include "PrismPrecompiledHeader.h"
#include "Prism/Core/Input.h"
#include "GLFW/glfw3.h"
#include "Prism/Core/Application.h"

namespace Prism
{
	bool Input::IsKeyPressed(int keyCode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		auto keyState = glfwGetKey(window, keyCode);
		return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int mouseButton)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		auto mouseState = glfwGetMouseButton(window, mouseButton);
		return mouseState == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow().GetNativeWindow());
		double xPosition, yPosition;
		glfwGetCursorPos(window, &xPosition, &yPosition);

		return { (float)xPosition, (float)yPosition };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}