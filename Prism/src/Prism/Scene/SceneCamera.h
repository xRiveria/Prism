#pragma once
#include "Prism/Renderer/Camera.h"

namespace Prism
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFieldOfView, float nearClip, float farClip);

		void SetViewportSize(uint32_t newWidth, uint32_t newHeight);

		//==== Perspective ====

		float GetPerspectiveVerticalFieldOfView() const { return m_PerspectiveFieldOfView; }
		void SetPerspectiveVerticalFieldOfView(float newVerticalFieldOfView) { m_PerspectiveFieldOfView = newVerticalFieldOfView; RecalculateProjectionMatrix(); }

		float GetPerspectiveNearClip() const { return m_PerspectiveNearClip; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNearClip = nearClip; RecalculateProjectionMatrix(); }

		float GetPerspectiveFarClip() const { return m_PerspectiveFarClip; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFarClip = farClip; RecalculateProjectionMatrix(); }

		//==== Orthographgic ====

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float newSize) { m_OrthographicSize = newSize; RecalculateProjectionMatrix(); }

		float GetOrthographicNearClip() const { return m_OrthographicNearClip; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNearClip = nearClip; RecalculateProjectionMatrix(); }
		
		float GetOrthographicFarClip() const { return m_OrthographicFarClip; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFarClip = farClip; RecalculateProjectionMatrix(); }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjectionMatrix();  }

	private:
		void RecalculateProjectionMatrix();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFieldOfView = glm::radians(45.0f);
		float m_PerspectiveNearClip = 0.01f, m_PerspectiveFarClip = 1000.0f;
		
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNearClip = -1.0f, m_OrthographicFarClip = 1.0f;
	
		float m_AspectRatio = 0.0f;
	};
}
