#include "PrismPrecompiledHeader.h"
#include "LayerStack.h"

namespace Prism
{
	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) 
	{
		//While layers will be pushed into the stack as the last layer, it will never be in front of an overlay.
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer); //For layers, they get pushed into LayerInsert which changes whenever we add or remove a layer.
	}

	void LayerStack::PushOverlay(Layer* overlay) 
	{
		m_Layers.emplace_back(overlay); //As overlays are last, we will always push them to the very back of the list. 
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
		}
	}
}
