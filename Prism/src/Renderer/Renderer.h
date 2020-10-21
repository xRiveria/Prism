#pragma once
#include "RenderCommand.h"

namespace Prism
{
	class Renderer
	{
	public:
		static void BeginScene(); //To Do: Take in scene parameters.
		static void EndScene();

		static void SubmitToRenderQueue(const std::shared_ptr<VertexArray>& vertexArray);
		
		inline static RendererAPI::RenderAPI GetCurrentRenderAPI() { return RendererAPI::GetCurrentRenderAPI(); }
	};
}
