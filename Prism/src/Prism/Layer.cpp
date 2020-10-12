#include "PrismPrecompiledHeader.h"
#include "Layer.h"

namespace Prism
{
	Layer::Layer(const std::string& layerName) : m_DebugName(layerName)
	{
		PRISM_ENGINE_WARN("Created {0}", layerName);
	}

	Layer::~Layer()
	{

	}
}
