#pragma once
#include "entt.hpp"

namespace Prism
{
	class Scene
	{
	public:
		Scene();
		~Scene();

	private:
		entt::registry m_Registry; //The registry is a container for all our component data and entity IDs. Think of it as a entity context that contains our entities. Thats why we have 1 per scene.
		
	};
}