#include "PrismPrecompiledHeader.h"
#include "OrthographicCameraController.h"
#include "Prism/Core/Input.h"
#include "Prism/Core/KeyCodes.h"

namespace Prism
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool cameraRotationEnabled) : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_CameraRotationEnabled(cameraRotationEnabled) //Literally -1 to 1 Vertically, and (+/-)AspectRatio * m_ZoomLevel Horizontally.
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep timeStep)
	{
		if (Input::IsKeyPressed(PRISM_KEY_A))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * timeStep;
		}

		else if (Input::IsKeyPressed(PRISM_KEY_D))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * timeStep;
		}

		if (Input::IsKeyPressed(PRISM_KEY_W))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * timeStep;
		}

		else if (Input::IsKeyPressed(PRISM_KEY_S))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * timeStep;
		}

		if (m_CameraRotationEnabled == true)
		{
			if (Input::IsKeyPressed(PRISM_KEY_Q))
			{
				m_CameraRotation += m_CameraRotationSpeed * timeStep;
			}
			else if (Input::IsKeyPressed(PRISM_KEY_E))
			{
				m_CameraRotation -= m_CameraRotationSpeed * timeStep;
			}

			m_Camera.SetCameraRotation(m_CameraRotation);
		}

		m_Camera.SetCameraPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel; //The closer we zoom, the slower the camera movement is, and vice versa.
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(PRISM_BIND_EVENT_FUNCTION(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(PRISM_BIND_EVENT_FUNCTION(OrthographicCameraController::OnWindowResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);  //Returns the greater value of a and b. If they are equivalent, returns a. Thus, m_ZoomLevel will never go below 0.25f.
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& event)
	{
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}