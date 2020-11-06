#pragma once
#include "glm/glm.hpp"
#include "Prism/Renderer/Camera.h"

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

		operator glm::mat4&() { return m_Transform; }
		operator const glm::mat4&() const { return m_Transform; }
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
		Prism::Camera m_Camera;
		bool m_IsPrimaryCamera = true; //To Do: Moving to scene.

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projectionMatrix) : m_Camera(projectionMatrix) {}
	};
}