#pragma once
#include "Renderer/Buffer.h"

namespace Prism
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void BindVertexBuffer() const override;
		virtual void UnbindVertexBuffer() const override;
		
	private:
		//Don't store any data in the main classes. Store them in the derived classes.
		uint32_t m_VertexBufferID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		virtual void BindIndexBuffer() const override;
		virtual void UnbindIndexBuffer() const override;

		virtual uint32_t GetIndicesCount() const override { return m_IndicesCount; }

	private:
		uint32_t m_IndexBufferID;
		uint32_t m_IndicesCount;
	};
}
