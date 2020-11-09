#pragma once

#include "PrismPrecompiledHeader.h"
#include "ImGuiLayer.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "examples/imgui_impl_opengl3.cpp"
#include "examples/imgui_impl_glfw.cpp"

#include "Prism/Core/Application.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Prism/Core/Core.h"

namespace Prism
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer")
	{
		PRISM_ENGINE_WARN("Created {0}", GetName());
	}

	void ImGuiLayer::OnAttach()
	{
		PRISM_PROFILE_FUNCTION();

		//Setup ImGui Context
		IMGUI_CHECKVERSION();
        ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); 
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; //Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; //Enable multi-viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportNoTaskbarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 17.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 17.0f);

		//Setup Styling
		ImGui::StyleColorsDark(); 

		//When viewports are enabled, we tweak WindowRounding/WindowBg so Window platforms can look identifical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}	

		SetDarkThemeColors();
		io.FontDefault = io.Fonts->Fonts.back();

		Application& application = Application::GetApplication();
		GLFWwindow* window = static_cast<GLFWwindow*>(application.GetWindow().GetNativeWindow());

		//Setup Platform/Renderer Bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		PRISM_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		if (m_BlockEvents)
		{
			PRISM_PROFILE_FUNCTION();
			ImGuiIO& io = ImGui::GetIO();
			event.IsEventHandled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.IsEventHandled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::BeginImGuiRenderLoop()
	{
		PRISM_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		/*
		ImGuiIO& io = ImGui::GetIO();
		Application& application = Application::GetApplication();
		io.DisplaySize = ImVec2((float)application.GetWindow().GetWindowWidth(), (float)application.GetWindow().GetWindowHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		//Menu
		// Menu Bar

		static bool showDemoWindow = true;
		ImGui::ShowDemoWindow(&showDemoWindow);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+O")) {  }
				if (ImGui::MenuItem("Open Scene", "Ctrl+S")) {  }
				ImGui::Separator();
				if (ImGui::MenuItem("Save Current Scene", "Ctrl+S")) {   }
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {   }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit Prism")) {  }
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Assets"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+O")) {   }
				if (ImGui::MenuItem("Open Scene", "Ctrl+S")) {   }
				ImGui::Separator();
				if (ImGui::MenuItem("Save Current Scene", "Ctrl+S")) { }
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {  }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit Prism")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Objects"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+O")) {  }
				if (ImGui::MenuItem("Open Scene", "Ctrl+S")) {  }
				ImGui::Separator();
				if (ImGui::MenuItem("Save Current Scene", "Ctrl+S")) {  }
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {   }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit Prism")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+O")) {  }
				if (ImGui::MenuItem("Open Scene", "Ctrl+S")) {   }
				ImGui::Separator();
				if (ImGui::MenuItem("Save Current Scene", "Ctrl+S")) {   }
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {  }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit Prism")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About Prism Engine")) {  }
				ImGui::Separator();
				if (ImGui::MenuItem("Prism Manual")) {   }
				if (ImGui::MenuItem("Scripting Reference")) {  }
				ImGui::Separator();
				if (ImGui::MenuItem("Release Notes")) {  }
				if (ImGui::MenuItem("Software Licenses")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Report A Bug")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		//ImGui::SetWindowSize("Play Options", ImVec2(application.GetWindow().GetWindowWidth(), 40));
		//ImGui::Begin("Play Options", &showGraphicalWindow, ImGuiWindowFlags_NoTitleBar);
		//ImGui::SameLine(application.GetWindow().GetWindowWidth() / 2);
		//ImGui::Button("Play");
		//ImGui::SameLine(application.GetWindow().GetWindowWidth() / 2);
		//ImGui::Button("Pause");
		//ImGui::SameLine(application.GetWindow().GetWindowWidth() / 2, +50.0f);
		//ImGui::Button("Stop");
		//ImGui::End();

		ImGui::Begin("Console");
		ImGui::End();

		ImGui::Begin("Profiler");
		ImGui::End();
		*/
	}

	void ImGuiLayer::EndImGuiRenderLoop()
	{
		PRISM_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& application = Application::GetApplication();
		io.DisplaySize = ImVec2((float)application.GetWindow().GetWindowWidth(), (float)application.GetWindow().GetWindowHeight()); //For default viewport.
		
		//Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
}
