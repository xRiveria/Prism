#pragma once
#include "Prism.h"
#include "Panels/SceneHierarchyPanel.h"
#include "glm/glm.hpp"
#include <vector>
#include "imgui/imgui.h"
#include "Widgets/ConsoleWidget.h"
#include "Widgets/AssetsWidget.h"
#include "Utilities/FileDialog.h"
#include "Utilities/IconProvider.h"

namespace Prism
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(ApplicationVersion& applicationVersion, ThirdPartyLibrary& thirdPartyLibraries);
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep timeStep) override;
		virtual void OnImGuiRender() override;
		void EditorImGuiRenderContent();
		void ShowAboutWindow();
		virtual void OnEvent(Event& event) override;

		
	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:
		glm::vec4 m_SceneClearColor = { 0, 0, 0, 0.1 };
		Reference<Framebuffer> m_Framebuffer;
		Reference<Texture2D> m_CheckboardTexture;
		OrthographicCameraController m_CameraController;
		EditorCamera m_EditorCamera;

		Reference<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		bool m_ShowPrismAboutWindow = false;
		glm::vec2 m_ViewportSize = { 0, 0 };

		int m_GizmoType = -1;

		ApplicationVersion* m_ApplicationVersion;
		ThirdPartyLibrary* m_ThirdPartyLibrary;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ConsoleWidget m_ConsoleWidget;
		AssetsWidget m_AssetsWidget;

		//Model ourObject;
		//Reference<Shader> m_ModelShader;
	};
}