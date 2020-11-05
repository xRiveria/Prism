#pragma once
#include "Prism/Core/Core.h"

namespace Prism
{
	struct FramebufferSpecification
	{
		uint32_t bufferWidth, bufferHeight;
		uint32_t bufferSamples = 1;

		bool bufferSwapChainTarget = false; //Are we rendering the screen or not?
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default; //If we're creating a Framebuffer object, even though it may be called as an OpenGLFrameBuffer, its destructor calls will always be in Framebuffer. Thus, if it doesn't exist, the destructor in OpenGLFramebuffer will never be called. Thus, use virtual default destructors.
		virtual const FramebufferSpecification& GetFramebufferSpecifications() const = 0;
		//virtual FramebufferSpecification& GetFramebufferSpecifications() = 0; //A non-const version to notify us if anything needs to be rebuilt.

		virtual void BindFramebuffer() = 0;
		virtual void UnbindFramebuffer() = 0;
		virtual void ResizeFramebuffer(uint32_t newWidth, uint32_t newHeight) = 0;
		virtual uint32_t GetColorAttachmentID() const = 0;

		static Reference<Framebuffer> CreateFramebuffer(const FramebufferSpecification& bufferSpecifications);
	};
}
