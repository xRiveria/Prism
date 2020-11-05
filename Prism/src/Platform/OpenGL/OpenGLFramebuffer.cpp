#include "PrismPrecompiledHeader.h"
#include "OpenGLFramebuffer.h"
#include "glad/glad.h"

namespace Prism
{
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& bufferSpecifications) : m_Specifications(bufferSpecifications)
	{
		InvalidateFrameBuffer();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_FrameBufferID);
		glDeleteTextures(1, &m_ColorAttachmentID);
		glDeleteTextures(1, &m_DepthAttachmentID);
	}

	void OpenGLFramebuffer::InvalidateFrameBuffer()
	{
		if (m_FrameBufferID) //If a Framebuffer exists...
		{
			glDeleteFramebuffers(1, &m_FrameBufferID);
			glDeleteTextures(1, &m_ColorAttachmentID);
			glDeleteTextures(1, &m_DepthAttachmentID);
		}

		glCreateFramebuffers(1, &m_FrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specifications.bufferWidth, m_Specifications.bufferHeight, 0, GL_RGBA, GL_UNSIGNED_INT, nullptr);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentID, 0);
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachmentID);
		//glTexStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specifications.bufferWidth, m_Specifications.bufferHeight);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specifications.bufferWidth, m_Specifications.bufferHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachmentID, 0);

		PRISM_ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::BindFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
		glViewport(0, 0, m_Specifications.bufferWidth, m_Specifications.bufferHeight);
	}

	void OpenGLFramebuffer::UnbindFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::ResizeFramebuffer(uint32_t newWidth, uint32_t newHeight)
	{
		m_Specifications.bufferWidth = newWidth;
		m_Specifications.bufferHeight = newHeight;
		InvalidateFrameBuffer();
	}
}

