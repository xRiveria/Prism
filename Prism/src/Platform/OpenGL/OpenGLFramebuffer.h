#pragma once
#include "Prism/Renderer/Framebuffer.h"

namespace Prism
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& bufferSpecifications);
		virtual ~OpenGLFramebuffer();

		void InvalidateFrameBuffer(); //Recreate Framebuffer.

		virtual void BindFramebuffer() override;
		virtual void UnbindFramebuffer() override;
		virtual void ResizeFramebuffer(uint32_t newWidth, uint32_t newHeight) override;
		virtual uint32_t GetColorAttachmentID() const override { return m_ColorAttachmentID; }

		virtual const FramebufferSpecification& GetFramebufferSpecifications() const override { return m_Specifications; }

	private:
		uint32_t m_FrameBufferID = 0;
		uint32_t m_ColorAttachmentID = 0, m_DepthAttachmentID = 0;
		FramebufferSpecification m_Specifications;
	};
}