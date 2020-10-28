#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Prism/Renderer/Renderer.h"
#include <chrono>

//To Do: ImGuii feature that essentially when button is clicked on, profile the next 10 seconds or until stop is pressed, and outputs to a .json file.

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	PRISM_PROFILE_FUNCTION();

	m_CheckboardTexture = Prism::Texture2D::CreateTexture("assets/textures/Checkerboard.png");
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
		Prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Prism::RenderCommand::Clear();
	}

	{
		PRISM_PROFILE_SCOPE("Rendering Draw");

		static float rotation = 0.0f;
		rotation += timeStep * 50.0f;

		Prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Prism::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, rotation, { m_SquareColor });
		Prism::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.75f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Prism::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Prism::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckboardTexture, 10.0f);
		Prism::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckboardTexture, 20.0f);

		Prism::Renderer2D::EndScene();

		Prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Prism::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}

		Prism::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	PRISM_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	Prism::Renderer2D::Statistics batchingStatistics = Prism::Renderer2D::GetBatchingStatistics();

	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", batchingStatistics.m_DrawCalls);
	ImGui::Text("Quads: %d", batchingStatistics.m_QuadCount);
	ImGui::Text("Vertices: %d", batchingStatistics.GetTotalVertexCount());
	ImGui::Text("Indices: %d", batchingStatistics.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));	
	ImGui::End();
}

void Sandbox2D::OnEvent(Prism::Event& event)
{
	m_CameraController.OnEvent(event);
}


