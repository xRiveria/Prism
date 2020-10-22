#pragma once
#include "RenderCommand.h"
#include "Renderer/Shader.h"
#include "OrthographicCamera.h"

namespace Prism
{
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera); //To Do: Take in scene parameters.
		static void EndScene();

		static void SubmitToRenderQueue(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
		
		inline static RendererAPI::RenderAPI GetCurrentRenderAPI() { return RendererAPI::GetCurrentRenderAPI(); }
	
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}
