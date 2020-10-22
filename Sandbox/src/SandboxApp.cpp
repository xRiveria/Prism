#include "Prism.h"
#include "imgui/imgui.h"

class ExampleLayer : public Prism::Layer
{
public:
	ExampleLayer():Layer("Example Layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		PRISM_CLIENT_WARN("Created {0}", GetName());

		//=================================================================
		// Triangle
		//=================================================================

		//Vertex Array
		m_VertexArray.reset(Prism::VertexArray::CreateVertexArray());
		m_VertexArray->BindVertexArray();

		float vertices[3 * 7] = //Within clip space already - no MVP needed.
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Prism::VertexBuffer> m_VertexBuffer;
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
		std::shared_ptr<Prism::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Prism::IndexBuffer::CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
		m_IndexBuffer->BindIndexBuffer();

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		//Shader
		std::string vertexShaderSourceCode = R"(
		#version 330 core
		layout (location = 0) in vec3 attributePosition;
		layout (location = 1) in vec4 attributeColor;

		uniform mat4 u_ViewProjection;
		out vec3 outputPosition;
		out vec4 v_Color;

		void main()
		{
			v_Color = attributeColor;
			outputPosition = attributePosition;
			gl_Position = u_ViewProjection * vec4(attributePosition, 1.0f);
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

		m_Shader.reset(new Prism::Shader(vertexShaderSourceCode, fragmentShaderSourceCode));

		//=================================================================
		// Square
		//=================================================================

		m_SquareVertexArray.reset(Prism::VertexArray::CreateVertexArray());
		m_SquareVertexArray->BindVertexArray();

		float squareVertices[3 * 4] =
		{
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Prism::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Prism::VertexBuffer::CreateVertexBuffer(squareVertices, sizeof(squareVertices)));
		Prism::BufferLayout blueLayout =
		{
			{ Prism::ShaderDataType::Float3, "a_Position", false },
		};

		squareVertexBuffer->SetBufferLayout(blueLayout);
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6]
		{
			0, 1, 2, 2, 3, 0
		};

		//Index Buffer
		std::shared_ptr<Prism::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Prism::IndexBuffer::CreateIndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		squareIndexBuffer->BindIndexBuffer();

		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		//Some graphic cards have default shaders created. Thus, we do not have to create them to start rendering.
		//However, in more contexts, we will have to create shaders if we want to do anything remotely interesting.
		std::string blueVertexShaderSourceCode = R"(
		#version 330 core
		layout (location = 0) in vec3 attributePosition;

		uniform mat4 u_ViewProjection;
		out vec3 outputPosition;

		void main()
		{
			outputPosition = attributePosition;
			gl_Position = u_ViewProjection * vec4(attributePosition, 1.0f);
		}
		)";

		std::string blueFragmentShaderSourceCode = R"(
		#version 330 core
		
		in vec3 outputPosition;
		out vec4 outputColor;

		void main()
		{
			outputColor = vec4(outputPosition, 1.0f);
		}
		)";

		//Shader
		m_BlueShader.reset(new Prism::Shader(blueVertexShaderSourceCode, blueFragmentShaderSourceCode));
	}

	void OnUpdate() override
	{
		if (Prism::Input::IsKeyPressed(PRISM_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}

		else if (Prism::Input::IsKeyPressed(PRISM_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed;
		}

		if (Prism::Input::IsKeyPressed(PRISM_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMoveSpeed;
		}

		else if (Prism::Input::IsKeyPressed(PRISM_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}

		if (Prism::Input::IsKeyPressed(PRISM_KEY_A))
		{
			m_CameraRotation += m_CameraRotationSpeed;
		}
		else if (Prism::Input::IsKeyPressed(PRISM_KEY_D))
		{
			m_CameraRotation -= m_CameraRotationSpeed;
		}

		Prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0 });
		Prism::RenderCommand::Clear();

		m_Camera.SetCameraPosition(m_CameraPosition);
		m_Camera.SetCameraRotation(m_CameraRotation);

		//Renderer::BeginScene(camera, lights, environment);
		Prism::Renderer::BeginScene(m_Camera);

		Prism::Renderer::SubmitToRenderQueue(m_BlueShader, m_SquareVertexArray);
		Prism::Renderer::SubmitToRenderQueue(m_Shader, m_VertexArray);

		Prism::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(Prism::Event& event) override
	{
		
	}

private:
	std::shared_ptr<Prism::Shader> m_Shader;
	std::shared_ptr<Prism::VertexArray> m_VertexArray;

	std::shared_ptr<Prism::Shader> m_BlueShader;
	std::shared_ptr<Prism::VertexArray> m_SquareVertexArray;

	Prism::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;

	float m_CameraMoveSpeed = 0.1f;
	float m_CameraRotationSpeed = 2.0f;
};

class Sandbox : public Prism::Application
{
public:
	Sandbox()
	{
		PRISM_CLIENT_WARN("Created Sandbox Application");
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Prism::Application* Prism::CreateApplication()
{
	return new Sandbox();
}



