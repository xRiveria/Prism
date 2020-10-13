#pragma once
#include "Prism/Core.h"

namespace Prism
{
	//Input will be global. We will have 1 static input manager for our entire application.
	//As usual, this class will contain no actual data. 
	class PRISM_API Input
	{
	public: 
		inline static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImplementation(keyCode); } //Checks if a specific key is pressed.
		inline static bool IsMouseButtonPressed(int mouseButton) { return s_Instance->IsMouseButtonPressedImplementation(mouseButton); } //Checks if a specific mouse button is pressed.
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImplementation(); } //Gets the current mouse positions.
		inline static float GetMouseX() { return s_Instance->GetMouseXImplementation(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImplementation(); }
	
	protected:
		//To be inherited per platform.
		virtual bool IsKeyPressedImplementation(int keyCode) = 0;
		virtual bool IsMouseButtonPressedImplementation(int mouseButton) = 0;
		virtual std::pair<float, float> GetMousePositionImplementation() = 0;
		virtual float GetMouseXImplementation() = 0;
		virtual float GetMouseYImplementation() = 0;

	private:
		static Input* s_Instance;
	};


}
