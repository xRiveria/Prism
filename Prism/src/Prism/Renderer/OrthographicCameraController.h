#pragma once
#include "Prism/Renderer/OrthographicCamera.h"
#include "Prism/Core/Timestep.h"
#include "Prism/Events/ApplicationEvent.h"
#include "Prism/Events/MouseEvent.h"

namespace Prism
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool cameraRotationEnabled = false); //aspectRatio * 2 units.

		void OnUpdate(Timestep timeStep);
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		
		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float zoomLevel) 
		{ 
			m_ZoomLevel = zoomLevel; 
			CalculateViewProjection();
		}

	private:
		void CalculateViewProjection();

		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		//Remember that the order of initialization here matters even if we do initialize properly in the constructor,  
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		bool m_CameraRotationEnabled = false;

		float m_CameraTranslationSpeed = 5.0f; //We can make this a function. Faster if we're zoomed further away or slower when we're zoomed in.
		float m_CameraRotationSpeed = 180.0f;
	};
}

