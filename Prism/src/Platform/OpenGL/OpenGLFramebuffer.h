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
		virtual uint32_t GetColorAttachmentID() const override { return m_ColorAttachmentID; }

		virtual const FramebufferSpecification& GetFramebufferSpecifications() const override { return m_Specifications; }

	private:
		uint32_t m_FrameBufferID;
		uint32_t m_ColorAttachmentID, m_DepthAttachmentID;
		FramebufferSpecification m_Specifications;
	};
}