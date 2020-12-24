#include "PrismPrecompiledHeader.h"
#include "EditorCamera.h"
#include "Prism/Core/Input.h"
#include "Prism/Core/KeyCodes.h"
#include "Prism/Core/MouseButtonCodes.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Prism
{
	EditorCamera::EditorCamera(float fieldOfView, float aspectRatio, float nearClip, float farClip) 
		: m_FieldOfView(fieldOfView), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(glm::radians(fieldOfView), aspectRatio, nearClip, farClip))
	{
		UpdateViewMatrix();
	}

	void EditorCamera::UpdateProjectionMatrix()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FieldOfView), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateViewMatrix()
	{
		//m_Yaw = m_Pitch = 0.0f; //Lock the camera's rotation.
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); //Max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f; //Specially curated speed affecting factors.

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); //Max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f; //Specially curated speed affecting factors.

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const //Faster when further away, slower while closer.
	{
		float distance = m_Distance * 0.2f; 
		distance = std::max(distance, 0.0f);

		float speed = distance * distance;
		speed = std::min(speed, 100.0f); //Max speed = 100.

		return speed;
	}


	void EditorCamera::OnUpdate(Timestep deltaTime)
	{
		if (Input::IsKeyPressed(PRISM_KEY_LEFT_ALT))
		{
			const glm::vec2& currentMousePosition = { Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (currentMousePosition - m_InitialMousePosition) * 0.003f; //Offset
			m_InitialMousePosition = currentMousePosition;

			if (Input::IsMouseButtonPressed(PRISM_MOUSE_BUTTON_MIDDLE))
			{
				MousePan(delta);
			}
			else if (Input::IsMouseButtonPressed(PRISM_MOUSE_BUTTON_LEFT))
			{
				MouseRotate(delta);
			}
			else if (Input::IsMouseButtonPressed(PRISM_MOUSE_BUTTON_RIGHT))
			{
				MouseZoom(delta.y);
			}
		}

		UpdateViewMatrix();
	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(PRISM_BIND_EVENT_FUNCTION(EditorCamera::OnMouseScoll));
	}

	void EditorCamera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		UpdateProjectionMatrix();
	}

	bool EditorCamera::OnMouseScoll(MouseScrolledEvent& event)
	{
		float delta = event.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateViewMatrix();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

}