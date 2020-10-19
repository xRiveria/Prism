#include "PrismPrecompiledHeader.h"
#include "Application.h"
#include "glad/glad.h"
#include "Platform/Windows/WindowsInput.h"
#include "Prism/ImGui/ImGuiLayer.h"

namespace Prism
{
	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_FLOAT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		PRISM_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

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

		float vertices[3 * 7] = //Within clip space already - no MVP needed.
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices)));
		
		{
			BufferLayout layout =
			{
				{ ShaderDataType::Float3, "a_Position", false },
				{ ShaderDataType::Float4, "a_Color", false }
			};

			m_VertexBuffer->SetBufferLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.elementType), element.elementNormalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.elementOffset);
			index++;
		}
		
		m_VertexBuffer->BindVertexBuffer();

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
		layout (location = 1) in vec4 attributeColor;

		out vec3 outputPosition;
		out vec4 v_Color;

		void main()
		{
			v_Color = attributeColor;
			outputPosition = attributePosition;
			gl_Position = vec4(attributePosition, 1.0f);
		}
		)";

		std::string fragmentShaderSourceCode = R"(
		#version 330 core
		
		in vec3 outputPosition;
		in vec4 v_Color;
		out vec4 outputColor;

		void main()
		{
			outputColor = v_Color;
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
