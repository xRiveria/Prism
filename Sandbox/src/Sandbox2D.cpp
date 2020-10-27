#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Prism/Renderer/Renderer.h"
#include <chrono>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_CheckboardTexture = Prism::Texture2D::CreateTexture("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Prism::Timestep timeStep)
{
	PRISM_PROFILE_FUNCTION();

	//Update
	{
		PRISM_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(timeStep);
	}

	//Render
	{
		PRISM_PROFILE_SCOPE("Rendering Initialization");
		Prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Prism::RenderCommand::Clear();
	}

	{
		PRISM_PROFILE_SCOPE("Rendering Draw");
		Prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Prism::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { m_SquareColor });
		Prism::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Prism::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.5f, 10.5f }, m_CheckboardTexture);

		Prism::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	PRISM_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));	
	ImGui::End();
}

void Sandbox2D::OnEvent(Prism::Event& event)
{
	m_CameraController.OnEvent(event);
}


