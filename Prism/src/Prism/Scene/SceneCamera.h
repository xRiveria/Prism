#pragma once
#include "Prism/Renderer/Camera.h"

namespace Prism
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t newWidth, uint32_t newHeight);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float newSize) { m_OrthographicSize = newSize; RecalculateProjectionMatrix(); }

	private:
		void RecalculateProjectionMatrix();

	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNearClip = -1.0f, m_OrthographicFarClip = 1.0f;
	
		float m_AspectRatio = 0.0f;
	};
}
