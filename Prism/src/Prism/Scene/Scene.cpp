#include "PrismPrecompiledHeader.h"
#include "Scene.h"
#include "glm/glm.hpp"

namespace Prism
{
	struct SpriteComponent
	{

	};

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{
		//Find out which registry and which entity constructed the Transform component and do stuff etc. 
	}

	struct TransformComponent //How we define components. Its a pure struct of data. 
	{
		glm::mat4 componentTransform;
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 transform) : componentTransform(transform) {}
		
		operator glm::mat4() { return componentTransform; }
		operator const glm::mat4() const { return componentTransform; }
	};

	Scene::Scene()
	{
		entt::entity entity = m_Registry.create(); //This is how we create an Entity. Remember that entities are just an unsigned integer. (Identifier)
		auto& transform = m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f)); //This registers components on the fly instead of needing to register them manually with a function.
		
		if (m_Registry.has<TransformComponent>(entity)) //Gets a component from n entity. 
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}

		auto view = m_Registry.view<TransformComponent>(); //Gets all entities with the given component.
		for (auto entity : view)
		{
			//Render all their meshes whatsoever. 
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
			//Renderer::Submit(sprite, transform);
		}

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>(); //Hooks callbacks when an entity is created or destroyed.

#if UsefulStuff
		m_Registry.remove<TransformComponent>(entity);
		m_Registry.clear();
#endif
	}

	Scene::~Scene()
	{

	}
}