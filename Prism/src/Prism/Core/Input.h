#pragma once
#include "Prism/Core/Core.h"

namespace Prism
{
	//Input will be global. We will have 1 static input manager for our entire application.
	//As usual, this class will contain no actual data. 
	class PRISM_API Input
	{
	public: 
		//Keyboard
		static bool IsKeyPressed(int keyCode);
		
		//Mouse
		static bool IsMouseButtonPressed(int mouseButton);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
