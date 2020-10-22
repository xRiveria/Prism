#pragma once

#include "Prism/Core.h"
#include "Prism/Events/Event.h"
#include "Prism/Core/Timestep.h"

namespace Prism
{
	//Inherit this class to create new layers. 
	class PRISM_API Layer
	{
	public:
		Layer(const std::string& layerName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timeStep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName; //Names are for debug purposes. Names not really needed for layers.
	};
}
