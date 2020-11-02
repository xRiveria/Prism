#pragma once
#include "RenderCommand.h"
#include "Prism/Renderer/Shader.h"
#include "OrthographicCamera.h"

namespace Prism
{
	class Renderer
	{
	public:
		static void InitializeRenderer();
		static void OnWindowResize(uint32_t windowWidth, uint32_t windowHeight);

		static void BeginScene(OrthographicCamera& camera); //To Do: Take in scene parameters.
		static void EndScene();

		static void SubmitToRenderQueue(const Reference<Shader>& shader, const Reference<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f)); //Stick identity matrix here so we can choose not to add a transform component if we want = default origin. 
		
		inline static RendererAPI::RenderAPI GetCurrentRenderAPI() { return RendererAPI::GetCurrentRenderAPI(); }
	
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}
