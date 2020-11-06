#include "PrismPrecompiledHeader.h"
#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Prism
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_OrthographicNearClip = nearClip;
		m_OrthographicFarClip = farClip;

		RecalculateProjectionMatrix();
	}

	void SceneCamera::SetViewportSize(uint32_t newWidth, uint32_t newHeight)
	{
		m_AspectRatio = (float)newWidth / (float)newHeight;		
		RecalculateProjectionMatrix();
	}

	void SceneCamera::RecalculateProjectionMatrix()
	{
		float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = m_OrthographicSize * 0.5f;

		m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNearClip, m_OrthographicFarClip);
	}

}