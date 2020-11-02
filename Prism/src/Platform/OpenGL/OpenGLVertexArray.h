#pragma once
#include "Prism/Renderer/VertexArray.h"

namespace Prism
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();
		 
		virtual void BindVertexArray() const override;
		virtual void UnbindVertexArray() const override; 

		virtual void AddVertexBuffer(Reference<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(Reference<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Reference<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Reference<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		std::vector<Reference<VertexBuffer>> m_VertexBuffers;
		Reference<IndexBuffer> m_IndexBuffer;
		uint32_t m_VertexArrayID;
		uint32_t m_VertexBufferIndex = 0;
	};
}