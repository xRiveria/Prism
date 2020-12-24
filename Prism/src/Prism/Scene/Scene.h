#pragma once
#include "entt.hpp"
#include "Prism/Core/Timestep.h"
#include "glm/glm.hpp"
#include "Prism/Renderer/EditorCamera.h"

namespace Prism
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();
		 
		Entity CreateEntity(const std::string& entityName = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep deltaTime);
		
		void OnUpdateEditor(Timestep deltaTime, EditorCamera& camera); 

		void OnViewportResize(uint32_t newWidth, uint32_t newHeight);

		Entity GetPrimaryCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry; //The registry is a container for all our component data and entity IDs. Think of it as a entity context that contains our entities. Thats why we have 1 per scene.		
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}