#pragma once

#include "glm/glm.hpp"

namespace Prism
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float leftBound, float rightBound, float bottomBound, float topBound);

		void SetProjection(float leftBound, float rightBound, float bottomBound, float topBound);

		const glm::vec3& GetCameraPosition() const { return m_Position; }
		void SetCameraPosition(const glm::vec3& cameraPosition) { m_Position = cameraPosition; RecalculateViewMatrix(); }
		
		const float& GetCameraRotation() const { return m_Rotation; }
		void SetCameraRotation(float cameraZRotation) { m_Rotation = cameraZRotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix(); //If the camera reacts to mouse movements/event systems.

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix; //Inverse of the Transformation Matrix of the camera.
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f; //We can't really rotate orthographic cameras (2D) except its Z-axis, so we just store that as a single value. 
	}; 

	//Seperating data (OrthographicCamera) that is submitted to the renderer from user input data. Since this camera class is submitted to the renderer, we should keep it lightweight.
	//Input Controllers should be nowhere near renderer code. Its just a user interaction module that manipulates data.
}