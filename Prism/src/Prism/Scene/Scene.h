#pragma once
#include "entt.hpp"
#include "Prism/Core/Timestep.h"

namespace Prism
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& entityName = std::string());

		void OnUpdate(Timestep deltaTime);

	private:
		entt::registry m_Registry; //The registry is a container for all our component data and entity IDs. Think of it as a entity context that contains our entities. Thats why we have 1 per scene.		
		friend class Entity;
	};
}