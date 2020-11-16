#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Prism/Scene/SceneSerializer.h"
#include "Prism/Utilities/PlatformUtilities.h"

//Sandbox is going to become our runtime application.
//We create a game in PrismEditor, which will create a whole bunch of game data in which Sandbox will load that information in.
//We will create our tools and game in PrismEditor itself.
//Remember that we do not want to have any 3rd party code in here other than Glm. The rest should all be Prism Code.

namespace Prism
{
	//To Do: ImGui feature that essentially when button is clicked on, profile the next 10 seconds or until stop is pressed, and outputs to a .json file.
	EditorLayer::EditorLayer() : Layer("Editor Layer"), m_CameraController(1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach()
	{
		PRISM_PROFILE_FUNCTION();

		m_CheckboardTexture = Texture2D::CreateTexture("assets/textures/Checkerboard.png");
	
		FramebufferSpecification framebufferSpecification;
		framebufferSpecification.bufferWidth = 1280;
		framebufferSpecification.bufferHeight = 720;
		m_Framebuffer = Framebuffer::CreateFramebuffer(framebufferSpecification);
		
		//Entity
		m_ActiveScene = CreateReference<Scene>();

#if 0
		auto square = m_ActiveScene->CreateEntity("Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
		
		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.m_IsPrimaryCamera = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate() 
			{
				auto& transformComponentTranslation = GetComponent<TransformComponent>().m_Translation;
				transformComponentTranslation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{

			}

			void OnUpdate(Timestep deltaTime)
			{
				auto& transformComponentTranslation = GetComponent<TransformComponent>().m_Translation;
				float cameraSpeed = 5.0f;

				if (Input::IsKeyPressed(PRISM_KEY_A))
				{
					transformComponentTranslation.x -= cameraSpeed * deltaTime;
				}
			    if (Input::IsKeyPressed(PRISM_KEY_D))
				{
					transformComponentTranslation.x += cameraSpeed * deltaTime;
				}
				if (Input::IsKeyPressed(PRISM_KEY_W))
				{
					transformComponentTranslation.y += cameraSpeed * deltaTime;
				}
				if (Input::IsKeyPressed(PRISM_KEY_S))
				{
					transformComponentTranslation.y -= cameraSpeed * deltaTime;
				}
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().BindClass<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().BindClass<CameraController>();
#endif
		m_SceneHierarchyPanel.SetHierachyContext(m_ActiveScene);	
	}

	void EditorLayer::OnDetach()
	{
		PRISM_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep timeStep)
	{
		PRISM_PROFILE_FUNCTION();
		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetFramebufferSpecifications();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.bufferWidth != m_ViewportSize.x || spec.bufferHeight != m_ViewportSize.y)) // zero sized framebuffer is invalid
		{
			m_Framebuffer->ResizeFramebuffer((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		//Update
		if (m_ViewportFocused)
		{
			m_CameraController.OnUpdate(timeStep);
		}

		//Render
		//Reset our rendering statistics here as its the start of a new frame.
		Renderer2D::ResetBatchingStatistics();

		m_Framebuffer->BindFramebuffer();
		RenderCommand::SetClearColor(m_SceneClearColor);
		RenderCommand::Clear();

		//Update Scene
		m_ActiveScene->OnUpdate(timeStep);	

		m_Framebuffer->UnbindFramebuffer();
	}

	void EditorLayer::EditorImGuiRenderContent()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}

				ImGui::Separator();
				
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) 
				{ 
					Application::GetApplication().CloseApplication();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Customize"))
			{
				if (ImGui::MenuItem("Editor")) { }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Stats");
		Renderer2D::Statistics batchingStatistics = Renderer2D::GetBatchingStatistics();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", batchingStatistics.m_DrawCalls);
		ImGui::Text("Quads: %d", batchingStatistics.m_QuadCount);
		ImGui::Text("Vertices: %d", batchingStatistics.GetTotalVertexCount());
		ImGui::Text("Indices: %d", batchingStatistics.GetTotalIndexCount());
		ImGui::Spacing();

		ImGui::Text("Clear Color:");
		ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_SceneClearColor));
		ImGui::End();

		RenderCommand::DrawGraphicsInformation();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		if (m_ViewportFocused)
		{
			Application::GetApplication().GetImGuiLayer()->DoBlockEvents(!m_ViewportFocused || !m_ViewportHovered); //Allow or block events depending on which window is being hovered over or focused on.
		}

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentID();
		ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }); //0,0 in ImGui might be at the top, while for OpenGL it might be at the bottom.
		ImGui::End();
		ImGui::PopStyleVar(); //Pops the pushed style so other windows beyond this won't have the style's properties.

		ImGui::End();
	}

	void EditorLayer::OnImGuiRender()
	{
		PRISM_PROFILE_FUNCTION();

		static bool dockSpaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		float minimumWindowsize = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minimumWindowsize;


		EditorImGuiRenderContent();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(PRISM_BIND_EVENT_FUNCTION(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.GetRepeatCount() > 0)
		{
			return false;
		}

		bool controlPressed = Input::IsKeyPressed(PRISM_KEY_LEFT_CONTROL) || Input::IsKeyPressed(PRISM_KEY_RIGHT_CONTROL);
		bool shiftPressed = Input::IsKeyPressed(PRISM_KEY_LEFT_SHIFT) || Input::IsKeyPressed(PRISM_KEY_RIGHT_SHIFT);

		switch (event.GetKeyCode())
		{
			case PRISM_KEY_N:
			{
				if (controlPressed) { NewScene(); }
				break;
			}

			case PRISM_KEY_O:
			{
				if (controlPressed) { OpenScene(); }
				break;
			}

			case PRISM_KEY_S:
			{
				if (controlPressed && shiftPressed) { SaveSceneAs(); }
				break;
			}
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateReference<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetHierachyContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::optional<std::string> filePath = FileDialogs::OpenFile("Prism Scene (*.prism)\0*.prism\0"); //First half of the string is for display in the dialog box, the second half is the actual filter. 
		if (filePath)
		{
			m_ActiveScene = CreateReference<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetHierachyContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.DeserializeFromYAML(*filePath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::optional<std::string> filePath = FileDialogs::SaveFile("Prism Scene (*.prism)\0*.prism\0");
		if (filePath)
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.SerializeToYAML(*filePath);
		}
	}
}


