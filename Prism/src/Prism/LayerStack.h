#pragma once

#include "Prism/Core.h"
#include "Layer.h"
#include <vector>

namespace Prism
{
	class PRISM_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack(); //When the layer stack is destroyed, all layers are deleted.

		//In the future, if we change levels or scenes, it might be worth recreating the entire layer stack.

		void PushLayer(Layer* layer); //Layers are owned by the Layerstack once we give them to the stack. Thus, the layer stack remains until the application is shut down.
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer); //When we pop layers, we don't lose them. We only lose them when the layer stack is destroyed. Layers are meant to live throughout the lifetime of the application.
		void PopOverlay(Layer* overlay); //Even if we pop a layer, we can push them back in again later. It will just be removed from the vector but not deleted itself.

		//For updates in our layers. We will loop through all the layers in the stack.
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert; 
	};
}

