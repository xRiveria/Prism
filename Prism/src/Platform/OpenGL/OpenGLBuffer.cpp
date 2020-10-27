#include "PrismPrecompiledHeader.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Prism
{
	//============== Vertex Buffer ==============

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) //Batching.
	{
		PRISM_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); //We tell OpenGL that data is going to be written to the buffer every frame. While StaticDraw works, its going to be slower as this is more of a hint than anything else.
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		PRISM_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		PRISM_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_VertexBufferID);
	}

	void OpenGLVertexBuffer::BindVertexBuffer() const
	{
		PRISM_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	}

	void OpenGLVertexBuffer::UnbindVertexBuffer() const
	{
		PRISM_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//============== Index Buffer ==============

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_IndicesCount(count)
	{
		PRISM_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		PRISM_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_IndexBufferID);
	}

	void OpenGLIndexBuffer::BindIndexBuffer() const
	{
		PRISM_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	}

	void OpenGLIndexBuffer::UnbindIndexBuffer() const
	{
		PRISM_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}