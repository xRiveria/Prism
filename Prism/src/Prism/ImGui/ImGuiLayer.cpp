#include "PrismPrecompiledHeader.h"
#include "ImGuiLayer.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"
#include "Prism/Application.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Prism
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	static const char* ImGui_ImplGlfw_GetClipboardText(void* user_data)
	{
		return glfwGetClipboardString((GLFWwindow*)user_data);
	}

	static void ImGui_ImplGlfw_SetClipboardText(void* user_data, const char* text)
	{
		glfwSetClipboardString((GLFWwindow*)user_data, text);
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::GetStyle().WindowRounding = 0.0f;
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		
		ImFont* pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		io.FontDefault = io.Fonts->Fonts.back();

		//Temporary
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
		io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
		Application& application = Application::GetApplication();
		io.ClipboardUserData = application.GetWindow().GetConstructedWindow();

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& application = Application::GetApplication();
		io.DisplaySize = ImVec2(application.GetWindow().GetWindowWidth(), application.GetWindow().GetWindowHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		//Menu
	    // Menu Bar
		bool show = true;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Open Scene", "Ctrl+S")) { /* Do stuff */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Save Current Scene", "Ctrl+S")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) { /* Do stuff */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit Prism")) {  }
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Assets"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Open Scene", "Ctrl+S")) { /* Do stuff */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Save Current Scene", "Ctrl+S")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) { /* Do stuff */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit Prism")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Objects"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Open Scene", "Ctrl+S")) { /* Do stuff */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Save Current Scene", "Ctrl+S")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) { /* Do stuff */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit Prism")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Open Scene", "Ctrl+S")) { /* Do stuff */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Save Current Scene", "Ctrl+S")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) { /* Do stuff */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit Prism")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About Prism Engine")) { /* Do stuff */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Prism Manual")) { /* Do stuff */ }
				if (ImGui::MenuItem("Scripting Reference")) { /* Do stuff */ }
				ImGui::Separator();
				if (ImGui::MenuItem("Release Notes")) { /* Do stuff */ }
				if (ImGui::MenuItem("Software Licenses")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Report A Bug")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Debug Area");
		ImGui::BeginTabBar("Debug Area Tabs");

		ImGui::BeginTabItem("Console");
		ImGui::EndTabItem();

		ImGui::EndTabBar();
		ImGui::End();

		ImGui::SetWindowSize("Play Options", ImVec2(application.GetWindow().GetWindowWidth(), 40));	
		ImGui::Begin("Play Options", false, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SameLine(application.GetWindow().GetWindowWidth() / 2);
		ImGui::Button("Play");
		//ImGui::SameLine(application.GetWindow().GetWindowWidth() / 2);
		//ImGui::Button("Pause");
		//ImGui::SameLine(application.GetWindow().GetWindowWidth() / 2, +50.0f);
		//ImGui::Button("Stop");
		ImGui::End();

		static bool showGraphicalWindow = true;
		//Render Info
		ImGui::Begin("Graphical Information", &showGraphicalWindow);
		ImGui::Text("FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Renderer: %s", (char*)glGetString(GL_RENDERER));
		ImGui::Text("OpenGL Version: %s", (char*)glGetString(GL_VERSION));
		ImGui::Text("Card Vendor: %s", (char*)glGetString(GL_VENDOR));
		ImGui::End();

		static bool showDemoWindow = true;
		ImGui::ShowDemoWindow(&showDemoWindow);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event) //Writing some manually for clarity for future proofing. 
	{
		EventDispatcher eventDispatcher(event);
		eventDispatcher.Dispatch<MouseButtonPressedEvent>(PRISM_BIND_EVENT_FUNCTION(ImGuiLayer::OnMouseButtonPressedEvent));
		eventDispatcher.Dispatch<MouseButtonReleasedEvent>(PRISM_BIND_EVENT_FUNCTION(ImGuiLayer::OnMouseButtonReleasedEvent));
		eventDispatcher.Dispatch<MouseMovedEvent>(std::bind(&ImGuiLayer::OnMouseMovedEvent, this, std::placeholders::_1));
		eventDispatcher.Dispatch<MouseScrolledEvent>(std::bind(&ImGuiLayer::OnMouseScrolledEvent, this, std::placeholders::_1));
		eventDispatcher.Dispatch<KeyPressedEvent>(std::bind(&ImGuiLayer::OnKeyPressedEvent, this, std::placeholders::_1));
		eventDispatcher.Dispatch<KeyTypedEvent>(std::bind(&ImGuiLayer::OnKeyTypedEvent, this, std::placeholders::_1));
		eventDispatcher.Dispatch<KeyReleasedEvent>(std::bind(&ImGuiLayer::OnKeyReleasedEvent, this, std::placeholders::_1));
		eventDispatcher.Dispatch<WindowResizeEvent>(std::bind(&ImGuiLayer::OnWindowResizedEvent, this, std::placeholders::_1));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = true;

		return false; //For now, we don't want to consume every mouse event. We will let it go through all layers for now.
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(event.GetX(), event.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += event.GetXOffset();  //Horizontal Scroll
		io.MouseWheel += event.GetYOffset();   //Vertical Scroll

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = true;

		//Check to see if the follow modifers are pressed alongside the above key event.
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER]; //Cmd (Mac)/Windows Key
		
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event) //For ImGui Textbox
	{
		ImGuiIO& io = ImGui::GetIO(); 
		int keyCode = event.GetKeyCode();

		if (keyCode > 0 && keyCode < 0x10000)
		{
			io.AddInputCharacter((unsigned short)keyCode);
		}

		return false;
	}

	bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, event.GetWidth(), event.GetHeight());

		return false;
	}
}
