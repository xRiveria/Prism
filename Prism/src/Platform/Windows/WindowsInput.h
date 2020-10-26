#pragma once
#include "Prism/Core/Input.h"

namespace Prism
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImplementation(int keyCode) override;
		virtual bool IsMouseButtonPressedImplementation(int mouseButton) override;
		virtual std::pair<float, float> GetMousePositionImplementation() override;
		virtual float GetMouseXImplementation() override;
		virtual float GetMouseYImplementation() override;
	};
}

