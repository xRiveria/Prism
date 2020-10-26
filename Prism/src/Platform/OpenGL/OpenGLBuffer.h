#pragma once
#include "Prism/Renderer/Buffer.h"

namespace Prism
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void BindVertexBuffer() const override;
		virtual void UnbindVertexBuffer() const override;

		virtual void SetBufferLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		
	private:
		//Don't store any data in the main classes. Store them in the derived classes.
		uint32_t m_VertexBufferID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void BindIndexBuffer() const override;
		virtual void UnbindIndexBuffer() const override;

		virtual uint32_t GetIndicesCount() const override { return m_IndicesCount; }

	private:
		uint32_t m_IndexBufferID;
		uint32_t m_IndicesCount;
	};
}
