#pragma once

namespace Prism
{
	enum class RendererAPI
	{
		None = 0, 
		OpenGL = 1,
		DirectX = 2,
		Vulkan = 3
	};

	class Renderer
	{
	public:
		static inline RendererAPI GetCurrentRenderAPI() { return s_RendererAPI; }

	private:
		static RendererAPI s_RendererAPI;
	};
}
