#pragma once
#include "entt.hpp"
#include "Prism/Core/Timestep.h"

namespace Prism
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		void OnUpdate(Timestep deltaTime);

		//Temporary
		entt::registry& GetRegistry() { return m_Registry; }

	private:
		entt::registry m_Registry; //The registry is a container for all our component data and entity IDs. Think of it as a entity context that contains our entities. Thats why we have 1 per scene.		
	};
}