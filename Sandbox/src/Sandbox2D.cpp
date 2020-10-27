#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Prism/Renderer/Renderer.h"
#include <chrono>

template<typename Function>
class Timer
{
public:
	Timer(const char* timerName, Function&& function) : m_TimerName(timerName), m_Stopped(false), m_Function(function)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
		{
			Stop();
		}
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		auto duration = (end - start) * 0.001f;

		m_Function({ m_TimerName, duration });
	}

private:
	const char* m_TimerName;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	Function m_Function;
};

#define PROFILING_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

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
	PROFILING_SCOPE("Sandbox2D::OnUpdate");


	//Update
	{
		PROFILING_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(timeStep);
	}

	//Render
	{
		PROFILING_SCOPE("Renderer Preparation");
		Prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Prism::RenderCommand::Clear();
	}

	{
		PROFILING_SCOPE("Renderer Drawing");
		Prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Prism::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { m_SquareColor });
		Prism::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Prism::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.5f, 10.5f }, m_CheckboardTexture);

		Prism::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	
	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.m_ProfileName);
		ImGui::Text(label, result.m_ProfileTime); 
	}
	m_ProfileResults.clear();
	
	ImGui::End();
}

void Sandbox2D::OnEvent(Prism::Event& event)
{
	m_CameraController.OnEvent(event);
}


