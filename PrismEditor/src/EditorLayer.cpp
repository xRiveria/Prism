#include "EditorLayer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Prism/Scene/SceneSerializer.h"
#include "Prism/Utilities/PlatformUtilities.h"
#include "ImGuizmo.h"
#include "Prism/Math/Math.h"

//Sandbox is going to become our runtime application.
//We create a game in PrismEditor, which will create a whole bunch of game data in which Sandbox will load that information in.
//We will create our tools and game in PrismEditor itself.
//Remember that we do not want to have any 3rd party code in here other than Glm. The rest should all be Prism Code.

static float col_a = 220.0f;
static float col_b = 340.0f;

namespace Prism
{
	//To Do: ImGui feature that essentially when button is clicked on, profile the next 10 seconds or until stop is pressed, and outputs to a .json file.
	EditorLayer::EditorLayer(ApplicationVersion& applicationVersion, ThirdPartyLibrary& thirdPartyLibraries) : m_ApplicationVersion(&applicationVersion), m_ThirdPartyLibrary(&thirdPartyLibraries), Layer("Editor Layer"), m_CameraController(1280.0f / 720.0f)
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
		//m_ModelShader = Shader::CreateShader("assets/shaders/Texture.glsl");
		//ourObject.LoadModel("assets/models/backpack.obj");
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
					PRISM_ENGINE_INFO("Creating new Scene...");
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

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About Prism"))
				{
					m_ShowPrismAboutWindow = true;
				}
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Default"))
			{
				m_ActiveScene = CreateReference<Scene>();
				m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_SceneHierarchyPanel.SetHierachyContext(m_ActiveScene);

				SceneSerializer serializer(m_ActiveScene);
				serializer.DeserializeFromYAML("assets/scenes/WhiteCube.prism");
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ConsoleWidget.OnConsoleWidgetUpdate();
		m_AssetsWidget.OnAssetsWidgetUpdate();
		ShowAboutWindow();

		//ourObject.Draw(m_ModelShader);

		ImGui::Begin("Prism Version");
		ImGui::Text(m_ApplicationVersion->RetrieveApplicationVersion().c_str());
		ImGui::End();

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

		Application::GetApplication().GetImGuiLayer()->DoBlockEvents(!m_ViewportFocused && !m_ViewportHovered); //Allow or block events depending on which window is being hovered over or focused on.
		

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentID();
		ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }); //0,0 in ImGui might be at the top, while for OpenGL it might be at the bottom.
		
		//Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight); //Set to viewport and window size to adjust rect sizes.

			//Camera
			auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			const auto& cameraComponent = cameraEntity.GetComponent<CameraComponent>().m_Camera;
			const glm::mat4& cameraProjection = cameraComponent.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			//Entity Transform
			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent.GetTransform();

			//Snapping
			bool snap = Input::IsKeyPressed(PRISM_KEY_LEFT_CONTROL);
			float snapValue = 0.5f; //Snap to 0.5m for translation or scale.

			//Snap to 45 degrees for rotation.
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 45.0f;
			}

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);
			
			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Prism::Math::DecomposeTransform(transform, translation, rotation, scale);
				
				glm::vec3 deltaRotation = rotation - transformComponent.m_Rotation;
				
				transformComponent.m_Translation = translation;
				transformComponent.m_Rotation += deltaRotation;
				transformComponent.m_Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar(); //Pops the pushed style so other windows beyond this won't have the style's properties.

		ImGui::End();
	}

	void EditorLayer::ShowAboutWindow()
	{
		if (!m_ShowPrismAboutWindow)
		{
			return;
		}

		ImGui::SetNextWindowFocus();
		ImGui::Begin("About Prism", &m_ShowPrismAboutWindow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);

		ImGui::Text(m_ApplicationVersion->RetrieveApplicationVersion().c_str());
		ImGui::Text("Author: Ryan Tan");
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth());
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 55);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		if (ImGui::Button("GitHub"))
		{
			WindowsFileSystem::OpenDirectoryWindow("https://github.com/xRiveria/Prism-Engine-Reforged");
		}

		ImGui::Separator();

		ImGui::BeginChildFrame(ImGui::GetID("about_license"), ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 15.5f), ImGuiWindowFlags_NoMove);
		ImGui::Text("MIT License");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
		ImGui::Text("of this software and associated documentation files(the \"Software\"),");
		ImGui::Text("to deal in the Software without restriction, including without limitation");
		ImGui::Text("the rights to use, copy, modify, merge, publish, distribute, sublicense,");
		ImGui::Text("and / or sell copies of the Software, and to permit persons to whom the");
		ImGui::Text("Software is furnished to do so, subject to the following conditions:");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("The above copyright notice and this permission notice shall be included in");
		ImGui::Text("all copies or substantial portions of the Software.");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
		ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
		ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL");
		ImGui::Text("THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR");
		ImGui::Text("OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,");
		ImGui::Text("ARISING FROM, OUT OF OR INCONNECTION WITH THE SOFTWARE OR THE USE OR");
		ImGui::Text("OTHER DEALINGS IN THE SOFTWARE.");
		ImGui::EndChildFrame();

		ImGui::Separator();

		ImGui::Text("Third Party Libraries");
		{
			ImGui::Spacing();
			ImGui::Text("Name");
			ImGui::SameLine(col_a); ImGui::Text("Version");
			ImGui::SameLine(col_b); ImGui::Text("URL");
			ImGui::Spacing();
			for (const Library& library : m_ThirdPartyLibrary->GetThirdPartyLibraries())
			{
				ImGui::BulletText(library.libraryName.c_str());
				ImGui::SameLine(col_a); 
				ImGui::Text(library.libraryVersion.c_str());
				ImGui::SameLine(col_b); 
				ImGui::PushID(library.libraryURL.c_str());

				if (ImGui::Button(library.libraryURL.c_str()))
				{
					WindowsFileSystem::OpenDirectoryWindow(library.libraryURL);
				}

				ImGui::PopID();
			}			
		}
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

			//Gizmos
			case PRISM_KEY_Q:
				m_GizmoType = -1;
				break;

			case PRISM_KEY_W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;

			case PRISM_KEY_E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;

			case PRISM_KEY_R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
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


