#pragma once

#include "glm/glm.hpp"

namespace Prism
{
	class Camera
	{
	public:
		Camera(const glm::mat4& projectionMatrix) : m_Projection(projectionMatrix)
		{

		}

		const glm::mat4& GetProjection() const { return m_Projection; }
		
		//To Do::
		//void SetPerspective/SetOrthographic

	private:
		glm::mat4 m_Projection;
	};
}
