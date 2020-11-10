#pragma once
#include "Prism.h"
#include "Panels/SceneHierarchyPanel.h"
#include "glm/glm.hpp"
#include <vector>

namespace Prism
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep timeStep) override;
		virtual void OnImGuiRender() override;
		void EditorImGuiRenderContent();
		virtual void OnEvent(Event& event) override;
		
	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:
		//Temporary
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		Reference<Framebuffer> m_Framebuffer;
		Reference<Texture2D> m_CheckboardTexture;
		OrthographicCameraController m_CameraController;

		Reference<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}