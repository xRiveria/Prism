#pragma once

#include "Prism/Core/Layer.h"
#include "Prism/Events/MouseEvent.h"
#include "Prism/Events/KeyEvent.h"
#include "Prism/Events/ApplicationEvent.h"

namespace Prism
{
	class PRISM_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void BeginImGuiRenderLoop();
		virtual void OnImGuiRender() override; //This sits between Begin and End. All our ImGui render code goes here.
		void EndImGuiRenderLoop();

	private:
		float m_Time = 0.0f;
	};
}

