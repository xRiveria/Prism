#pragma once
#include "glm/glm.hpp"
#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Prism
{
	struct TagComponent
	{
		std::string m_Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : m_Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 m_Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : m_Transform(transform) {}

		operator glm::mat4& () { return m_Transform; }
		operator const glm::mat4& () const { return m_Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 m_Color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : m_Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera m_Camera;
		bool m_IsPrimaryCamera = true; //To Do: Moving to scene.
		bool m_IsAspectRatioFixed = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* m_EntityInstance = nullptr;

		ScriptableEntity* (*InstantiateScript)(); //Returns a ScriptableEntity.
		void(*DestroyScript)(NativeScriptComponent*); //Takes in a NativeScriptComponent.

		template<typename T>
		void BindClass()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nativeScriptComponent) { delete nativeScriptComponent->m_EntityInstance; nativeScriptComponent->m_EntityInstance = nullptr; };
		}
	};
}

#if NotUsingVirtual
struct NativeScriptComponent
{
	ScriptableEntity* m_EntityInstance = nullptr;

	std::function<void()> InstantiateFunction;
	std::function<void()> DestroyInstanceFunction;

	std::function<void(ScriptableEntity*)> OnCreateFunction;
	std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;
	std::function<void(ScriptableEntity*)> OnDestroyFunction;

	template<typename T>
	void BindClass()
	{
		InstantiateFunction = [&]() { m_EntityInstance = new T(); };
		DestroyInstanceFunction = [&]() { delete (T*)m_EntityInstance; m_EntityInstance = nullptr; };

		OnCreateFunction = [](ScriptableEntity* entityInstance) { ((T*)entityInstance)->OnCreate(); };
		OnDestroyFunction = [](ScriptableEntity* entityInstance) { ((T*)entityInstance)->OnDestroy(); };
		OnUpdateFunction = [](ScriptableEntity* entityInstance, Timestep deltaTime) { ((T*)entityInstance)->OnUpdate(deltaTime); };
	}
};
#endif


