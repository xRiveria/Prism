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
		inline static void InitializeRenderer()
		{
			s_RendererAPI->InitializeRenderer();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t windowWidth, uint32_t windowHeight)
		{
			s_RendererAPI->SetViewport(x, y, windowWidth, windowHeight);
		}
		
		inline static void DrawIndexed(const Reference<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
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