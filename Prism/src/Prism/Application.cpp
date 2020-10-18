#include "PrismPrecompiledHeader.h"
#include "Application.h"
#include "glad/glad.h"
#include "Platform/Windows/WindowsInput.h"
#include "Prism/ImGui/ImGuiLayer.h"

namespace Prism
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		PRISM_ENGINE_ASSERT(!s_Instance, "Application already exists.");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::ConstructWindow()); //Explicit conversion here that converts the created Window's pointer from ConstructWindow() into a unique pointer that is returned here.
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));		
	
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//Vertex Array
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 3] = //Within clip space already - no MVP needed.
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		m_VertexBuffer.reset(VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices)));
		m_VertexBuffer->BindVertexBuffer();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		
		uint32_t indices[3]
		{
			0, 1, 2
		};

		//Index Buffer
		m_IndexBuffer.reset(IndexBuffer::CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
		m_IndexBuffer->BindIndexBuffer();
		
		//Some graphic cards have default shaders created. Thus, we do not have to create them to start rendering.
		//However, in more contexts, we will have to create shaders if we want to do anything remotely interesting.
		std::string vertexShaderSourceCode = R"(
		#version 330 core
		layout (location = 0) in vec3 attributePosition;

		out vec3 outputPosition;

		void main()
		{
			outputPosition = attributePosition;
			gl_Position = vec4(attributePosition, 1.0f);
		}
		)";

		std::string fragmentShaderSourceCode = R"(
		#version 330 core
		
		in vec3 outputPosition;
		out vec4 outputColor;

		void main()
		{
			outputColor = vec4(outputPosition * 0.5 + 0.5, 1.0f);
		}
		)";

		//Shader
		m_Shader.reset(new Shader(vertexShaderSourceCode, fragmentShaderSourceCode));
	}

	Application::~Application()
	{
		
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher eventDispatcher(event);
		//If the event that comes through the dispatcher is a WindowCloseEvent, we will call the binded function.
		eventDispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.IsEventHandled)
			{
				break;
			}
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->BindShader(); //Good practice to bind shaders first. Other APIs does this. 
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetIndicesCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->BeginImGuiRenderLoop();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			m_ImGuiLayer->EndImGuiRenderLoop();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& closeEvent)
	{
		m_Running = false;
		return true; //Handled.
	}
}
