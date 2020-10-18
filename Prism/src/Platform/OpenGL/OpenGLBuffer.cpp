#include "PrismPrecompiledHeader.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Prism
{
	//============== Vertex Buffer ==============

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_VertexBufferID);
	}

	void OpenGLVertexBuffer::BindVertexBuffer() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	}

	void OpenGLVertexBuffer::UnbindVertexBuffer() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//============== Index Buffer ==============

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_IndicesCount(count)
	{
		glCreateBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_IndexBufferID);
	}

	void OpenGLIndexBuffer::BindIndexBuffer() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	}

	void OpenGLIndexBuffer::UnbindIndexBuffer() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}