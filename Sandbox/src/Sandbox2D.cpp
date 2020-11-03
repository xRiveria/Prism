#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//To Do: ImGuii feature that essentially when button is clicked on, profile the next 10 seconds or until stop is pressed, and outputs to a .json file.

static uint32_t s_MapWidth = 24; //The maximum width of each row. 
static const char* s_MapTiles = 
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWDDDDDDDWWWWWWWWWW"
"WWWWDDDDDDDDDDWWWWWWWWWW"
"WWWDDDDDDDDDDDDWWWWWWWWW"
"WWWWWDDDDDDDDDDWWWWWWWWW"
"WWWWWWWDDDDDDDDWWWWWWWWW"
"WWWWWWWWDDDDDDDWWWWWWWWW"
"WWWWWWWWWDDDDDWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	PRISM_PROFILE_FUNCTION();

	m_CheckboardTexture = Prism::Texture2D::CreateTexture("assets/textures/Checkerboard.png");
	m_ChickenTexture = Prism::Texture2D::CreateTexture("assets/textures/Chicken.png");
	m_SpriteSheet = Prism::Texture2D::CreateTexture("assets/game/textures/RPGpack_sheet_2X.png");
	m_TextureStairs = Prism::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0.0f, 11.0f }, { 128.0f, 128.0f });
	m_TextureTree = Prism::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1,2 });

	m_MapWidth = s_MapWidth; 
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth; //Finds the maximum height of the map tiles. In this case: 337 / 24 = 14.02 = 14.
	s_TextureMapper['D'] = Prism::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 }); //Maps 'D' to the Dirt texture.
	s_TextureMapper['W'] = Prism::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 }); //Maps 'W' to the Water texture.

	m_CameraController.SetZoomLevel(5.0f);
}

void Sandbox2D::OnDetach()
{
	PRISM_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Prism::Timestep timeStep)
{
	PRISM_PROFILE_FUNCTION();

	//Update

	m_CameraController.OnUpdate(timeStep);
	
	//Render
	//Reset our rendering statistics here as its the start of a new frame.
	Prism::Renderer2D::ResetBatchingStatistics();

	{
		PRISM_PROFILE_SCOPE("Rendering Initialization");
		Prism::RenderCommand::SetClearColor(m_ClearColor);
		Prism::RenderCommand::Clear();
	}

	{
		PRISM_PROFILE_SCOPE("Rendering Draw");
#if 0
		static float rotation = 0.0f;
		rotation += timeStep * 50.0f;

		Prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Prism::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, rotation, { m_SquareColor });
		Prism::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.75f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Prism::Renderer2D::DrawQuad(m_ChickenPosition, m_ChickenScale, m_ChickenTexture);
		Prism::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckboardTexture, 10.0f);
		Prism::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckboardTexture, 20.0f);

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Prism::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}

		Prism::Renderer2D::EndScene();

#endif

		Prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (uint32_t y = 0; y < m_MapHeight; y++) //Take note of the way you are interating the chars due to memory layout (memory indirection).
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * m_MapWidth]; //Gets the correct memory offset. We are dealing with the map as a contigious block of memory as it will be faster.  

				Prism::Reference<Prism::SubTexture2D> texture; //Selected texture.
				if (s_TextureMapper.find(tileType) != s_TextureMapper.end()) //If the texture is found within the mapper...
				{
					texture = s_TextureMapper[tileType]; //Select it.
				}
				else
				{
					texture = m_TextureStairs; //Else, auto assign stairs to it. 
				} 
				Prism::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, y - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture); //Draw the quad. 
			}
		}

		//Prism::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_TextureBarrel);
		//Prism::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_TextureStairs);
		//Prism::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.5f }, { 1.0f, 2.0f }, m_TextureTree);
		Prism::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
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
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			if (ImGui::MenuItem("Exit")) { Prism::Application::GetApplication().CloseApplication(); }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");
	Prism::Renderer2D::Statistics batchingStatistics = Prism::Renderer2D::GetBatchingStatistics();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", batchingStatistics.m_DrawCalls);
	ImGui::Text("Quads: %d", batchingStatistics.m_QuadCount);
	ImGui::Text("Vertices: %d", batchingStatistics.GetTotalVertexCount());
	ImGui::Text("Indices: %d", batchingStatistics.GetTotalIndexCount());
	ImGui::Spacing();

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::Text("Clear Color:");
	ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
	ImGui::End();

	ImGui::Begin("Chicken Settings");
	ImGui::Text("Chicken Position:");
	ImGui::DragFloat3("Chicken Position", glm::value_ptr(m_ChickenPosition), 0.5f);
	ImGui::Text("Chicken Scale");
	ImGui::DragFloat2("Chicken Scale", glm::value_ptr(m_ChickenScale), 0.5f);
	uint32_t textureID = m_CheckboardTexture->GetTextureID();
	ImGui::Image((void*)textureID, { 256.0f, 256.0f });
	ImGui::End();

	ImGui::End();
}

void Sandbox2D::OnEvent(Prism::Event& event)
{
	m_CameraController.OnEvent(event);
}


