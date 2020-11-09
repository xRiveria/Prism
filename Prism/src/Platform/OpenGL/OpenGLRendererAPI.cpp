#include "PrismPrecompiledHeader.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"
#include "imgui.h"

namespace Prism
{
	void OpenGLRendererAPI::InitializeRenderer()
	{
		PRISM_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t windowWidth, uint32_t windowHeight)
	{
		glViewport(x, y, windowWidth, windowHeight);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawGraphicsInformation()
	{
		ImGui::Begin("Graphical Information");
		ImGui::Text("FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Renderer: %s", (char*)glGetString(GL_RENDERER));
		ImGui::Text("OpenGL Version: %s", (char*)glGetString(GL_VERSION));
		ImGui::Text("Card Vendor: %s", (char*)glGetString(GL_VENDOR));
		ImGui::End();
	}

	void OpenGLRendererAPI::DrawIndexed(const Reference<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetIndicesCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}