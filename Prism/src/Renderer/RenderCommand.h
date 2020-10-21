#pragma once
#include "RendererAPI.h"

namespace Prism
{
	//Render Command Queue.
	class RenderCommand
	{
	public:
		//Dispatch render commands to the respective selected API.
		//Render Commands don't do multiple things. They simply submit commands.
		//The Renderer class is the one that is in charge of binding, sorting etc. Once done, we submit the corresponding commands here.
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}