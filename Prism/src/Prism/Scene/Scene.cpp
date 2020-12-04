#include "PrismPrecompiledHeader.h"
#include "Scene.h"
#include "Components.h"
#include "Prism/Renderer/Renderer2D.h"
#include "Entity.h"

namespace Prism
{
	Scene::Scene()
	{

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

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
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
		glm::mat4 cameraTransform;

		{
			auto sceneCameras = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : sceneCameras)
			{
				auto [transform, camera] = sceneCameras.get<TransformComponent, CameraComponent>(entity);
				if (camera.m_IsPrimaryCamera)
				{
					mainCamera = &camera.m_Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera) //No rendering if camera doesn't exist.
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.m_Color);
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
			{
				cameraComponent.m_Camera.SetViewportSize(newWidth, newHeight);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.m_IsPrimaryCamera)
			{
				return Entity(entity, this);
			}
		}
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false); //Fail if a component is added that isn't being specialized below. 
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.m_Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}
}