#pragma once

#include "PrismPrecompiledHeader.h"
#include "ImGuiLayer.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "examples/imgui_impl_opengl3.cpp"
#include "examples/imgui_impl_glfw.cpp"

#include "Prism/Application.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Prism/Core.h"

namespace Prism
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer")
	{
		PRISM_ENGINE_WARN("Created {0}", GetName());
	}

	ImGuiLayer::~ImGuiLayer()
	{
	
	}

	void ImGuiLayer::OnAttach()
	{
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

		//Setup Styling
		ImGui::StyleColorsDark();

		//When viewports are enabled, we tweak WindowRounding/WindowBg so Window platforms can look identifical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}	

		ImFont* pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		io.FontDefault = io.Fonts->Fonts.back();

		Application& application = Application::GetApplication();
		GLFWwindow* window = static_cast<GLFWwindow*>(application.GetWindow().GetNativeWindow());

		//Setup Platform/Renderer Bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::BeginImGuiRenderLoop()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::OnImGuiRender()
	{
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


		static bool showGraphicalWindow = true;
		//Render Info
		ImGui::Begin("Graphical Information", &showGraphicalWindow);
		ImGui::Text("FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Renderer: %s", (char*)glGetString(GL_RENDERER));
		ImGui::Text("OpenGL Version: %s", (char*)glGetString(GL_VERSION));
		ImGui::Text("Card Vendor: %s", (char*)glGetString(GL_VENDOR));
		ImGui::End();

		ImGui::Begin("Console");
		ImGui::End();

		ImGui::Begin("Profiler");
		ImGui::End();
	}

	void ImGuiLayer::EndImGuiRenderLoop()
	{
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
}
