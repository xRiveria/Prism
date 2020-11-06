#pragma once

#include "glm/glm.hpp"

namespace Prism
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projectionMatrix) : m_ProjectionMatrix(projectionMatrix)
		{

		}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
		
		//To Do::
		//void SetPerspective/SetOrthographic

	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};
}
