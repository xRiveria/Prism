#pragma once
#include "Prism.h"
#include "glm/glm.hpp"
#include <vector>

namespace Prism
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep timeStep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		//Temporary
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		Reference<Texture2D> m_ChickenTexture;
		Reference<Framebuffer> m_Framebuffer;
		Reference<Texture2D> m_CheckboardTexture;
		Reference<SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;
		Reference<Texture2D> m_SpriteSheet;
		OrthographicCameraController m_CameraController;

		glm::vec2 m_ViewportSize = { 0, 0 };

		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Reference<SubTexture2D>> s_TextureMapper;
	};
}