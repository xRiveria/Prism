#pragma once
#include "Prism.h"
#include "glm/glm.hpp"
#include <vector>

class Sandbox2D : public Prism::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Prism::Timestep timeStep) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Prism::Event& event) override;

private:
	//Temporary
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

	Prism::Reference<Prism::Texture2D> m_ChickenTexture;
	Prism::Reference<Prism::Framebuffer> m_Framebuffer;
	Prism::Reference<Prism::Texture2D> m_CheckboardTexture;
	Prism::Reference<Prism::SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;
	Prism::Reference<Prism::Texture2D> m_SpriteSheet;
	Prism::OrthographicCameraController m_CameraController;

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Prism::Reference<Prism::SubTexture2D>> s_TextureMapper;
};