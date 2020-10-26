#include "Prism.h"
#include "Prism/Core/EntryPoint.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Sandbox2D.h"

class ExampleLayer : public Prism::Layer
{
public:
	ExampleLayer():Layer("Example Layer"), m_CameraController(1280.0f / 720.0f)
	{
		PRISM_CLIENT_WARN("Created {0}", GetName());

		//=================================================================
		// Triangle
		//=================================================================

		//Vertex Array
		m_VertexArray = Prism::VertexArray::CreateVertexArray();
		m_VertexArray->BindVertexArray();

		float vertices[3 * 7] = //Within clip space already - no MVP needed.
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Prism::Reference<Prism::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Prism::VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices)));

		Prism::BufferLayout layout =
		{
			{ Prism::ShaderDataType::Float3, "a_Position", false },
			{ Prism::ShaderDataType::Float4, "a_Color", false }
		};

		m_VertexBuffer->SetBufferLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3]
		{
			0, 1, 2
		};

		//Index Buffer
		Prism::Reference<Prism::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Prism::IndexBuffer::CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
		m_IndexBuffer->BindIndexBuffer();

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		//Shader
		std::string vertexShaderSourceCode = R"(
		#version 330 core
		layout (location = 0) in vec3 attributePosition;
		layout (location = 1) in vec4 attributeColor;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;
		out vec3 outputPosition;
		out vec4 v_Color;

		void main()
		{
			v_Color = attributeColor;
			outputPosition = attributePosition;
			gl_Position = u_ViewProjection * u_Transform * vec4(attributePosition, 1.0f);
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

		m_Shader = Prism::Shader::CreateShader("VertexColorTriangle", vertexShaderSourceCode, fragmentShaderSourceCode);

		//=================================================================
		// Square
		//=================================================================

		m_SquareVertexArray = Prism::VertexArray::CreateVertexArray();
		m_SquareVertexArray->BindVertexArray();

		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Prism::Reference<Prism::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Prism::VertexBuffer::CreateVertexBuffer(squareVertices, sizeof(squareVertices)));
		Prism::BufferLayout blueLayout =
		{
			{ Prism::ShaderDataType::Float3, "a_Position", false },
			{ Prism::ShaderDataType::Float2, "a_TextCoord" }
		};

		squareVertexBuffer->SetBufferLayout(blueLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6]
		{
			0, 1, 2, 2, 3, 0
		};

		//Index Buffer
		Prism::Reference<Prism::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Prism::IndexBuffer::CreateIndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		squareIndexBuffer->BindIndexBuffer();

		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		//Some graphic cards have default shaders created. Thus, we do not have to create them to start rendering.
		//However, in more contexts, we will have to create shaders if we want to do anything remotely interesting.
		std::string flatColorVertexShaderSourceCode = R"(
		#version 330 core
		layout (location = 0) in vec3 attributePosition;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;
		out vec3 outputPosition;

		void main()
		{
			outputPosition = attributePosition;
			gl_Position = u_ViewProjection * u_Transform * vec4(attributePosition, 1.0f);
		}
		)";

		std::string flatColorFragmentShaderSourceCode = R"(
		#version 330 core
		
		uniform vec3 u_Color;
		in vec3 outputPosition;
		out vec4 outputColor;

		void main()
		{
			outputColor = vec4(u_Color, 1.0f);
		}
		)";

		m_FlatColorShader = Prism::Shader::CreateShader("Flat Color", flatColorVertexShaderSourceCode, flatColorFragmentShaderSourceCode);
		//Shader
		Prism::Reference<Prism::Shader>& textureShader = m_ShaderLibrary.LoadShader("assets/shaders/Texture.glsl");
		
		m_PrismTexture = Prism::Texture2D::CreateTexture("assets/textures/PrismLogo.png");
		m_Texture = Prism::Texture2D::CreateTexture("assets/textures/Checkerboard.png");
		std::dynamic_pointer_cast<Prism::OpenGLShader>(textureShader)->BindShader();
		std::dynamic_pointer_cast<Prism::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
}

	void OnUpdate(Prism::Timestep timestep) override //In any implementation, if we have OnRender, it typically happens after OnUpdate. 
	{ 
		//Update
		m_CameraController.OnUpdate(timestep);

		//Render
		Prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0 });
		Prism::RenderCommand::Clear();

		//Renderer::BeginScene(camera, lights, environment);
		Prism::Renderer::BeginScene(m_CameraController.GetCamera());
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		/*Prism::MaterialReference material = new Prism::Material(m_FlatColorShader);
		Prism::MaterialInstanceReference materialReference = new Prism::MaterialInstance(material);

		materialReference->SetColorValue("u_Color", redColor);
		materialReference->SetTexture("u_AlbedoMap", texture);
		squareMesh->SetMaterial(materialReference);*/
		
		std::dynamic_pointer_cast<Prism::OpenGLShader>(m_FlatColorShader)->BindShader();
		std::dynamic_pointer_cast<Prism::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
				Prism::Renderer::SubmitToRenderQueue(m_FlatColorShader, m_SquareVertexArray, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.GetShader("Texture");
		//Texture
		m_Texture->BindTexture();
		Prism::Renderer::SubmitToRenderQueue(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_PrismTexture->BindTexture();
		Prism::Renderer::SubmitToRenderQueue(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//=================
		//Prism::Renderer::SubmitToRenderQueue(m_Shader, m_VertexArray, squarePosition);
		//Prism::Renderer::SubmitToRenderQueue(m_Shader, m_VertexArray);

		Prism::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Prism::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

private:
	Prism::ShaderLibrary m_ShaderLibrary;
	Prism::Reference<Prism::Shader> m_Shader;
	Prism::Reference<Prism::VertexArray> m_VertexArray;

	Prism::Reference<Prism::Shader> m_FlatColorShader;
	Prism::Reference<Prism::VertexArray> m_SquareVertexArray;

	Prism::Reference<Prism::Texture2D> m_Texture;
	Prism::Reference<Prism::Texture2D> m_PrismTexture; 

	Prism::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Prism::Application
{
public:
	Sandbox()
	{
		PRISM_CLIENT_WARN("Created Sandbox Application");
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	} 
};

Prism::Application* Prism::CreateApplication()
{
	return new Sandbox();
}





