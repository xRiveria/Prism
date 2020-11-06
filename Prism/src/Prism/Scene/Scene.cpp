#include "PrismPrecompiledHeader.h"
#include "Scene.h"
#include "glm/glm.hpp"
#include "Components.h"
#include "Prism/Renderer/Renderer2D.h"
#include "Entity.h"

namespace Prism
{
	Scene::Scene()
	{

#if UsefulStuff
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

		struct TransformComponent //How we define components. Its a pure struct of data. 
		{
			glm::mat4 componentTransform;
			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4 transform) : componentTransform(transform) {}

			operator glm::mat4() { return componentTransform; }
			operator const glm::mat4() const { return componentTransform; }
		};
		m_Registry.remove<TransformComponent>(entity);
		m_Registry.clear();
#endif
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& entityName)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>(); //Default component added.	
		auto& entityTag = entity.AddComponent<TagComponent>(); //Default component added.

		entityTag.m_Tag = entityName.empty() ? "Unnamed Entity" : entityName;

		return entity;
	}

	void Scene::OnUpdate(Timestep deltaTime)
	{
		//Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nativeScriptComponent)
				{
					//To Do: Move to Scene::OnScenePlay
					if (!nativeScriptComponent.m_EntityInstance)
					{
						nativeScriptComponent.m_EntityInstance = nativeScriptComponent.InstantiateScript();
						nativeScriptComponent.m_EntityInstance->m_Entity = { entity, this };
						nativeScriptComponent.m_EntityInstance->OnCreate();
					}

					nativeScriptComponent.m_EntityInstance->OnUpdate(deltaTime);
				});
		}

		//Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		{
			auto sceneCameras = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : sceneCameras)
			{
				auto [transform, camera] = sceneCameras.get<TransformComponent, CameraComponent>(entity);
				if (camera.m_IsPrimaryCamera)
				{
					mainCamera = &camera.m_Camera;
					cameraTransform = &transform.m_Transform;
					break;
				}
			}
		}

		if (mainCamera) //No rendering if camera doesn't exist.
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.m_Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t newWidth, uint32_t newHeight)
	{
		m_ViewportWidth = newWidth;
		m_ViewportHeight = newHeight;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.m_IsAspectRatioFixed)
				cameraComponent.m_Camera.SetViewportSize(newWidth, newHeight);
		}
	}

}