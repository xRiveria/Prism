#include "PrismPrecompiledHeader.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Prism
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_FLOAT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		PRISM_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		PRISM_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_VertexArrayID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		PRISM_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void OpenGLVertexArray::BindVertexArray() const
	{
		PRISM_PROFILE_FUNCTION();

		glBindVertexArray(m_VertexArrayID);
	}

	void OpenGLVertexArray::UnbindVertexArray() const
	{
		PRISM_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(Reference<VertexBuffer>& vertexBuffer)
	{
		PRISM_PROFILE_FUNCTION();

		PRISM_ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_VertexArrayID);
		vertexBuffer->BindVertexBuffer();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.elementType), element.elementNormalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(intptr_t)element.elementOffset);
			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(Reference<IndexBuffer>& indexBuffer)
	{
		PRISM_PROFILE_FUNCTION();

		glBindVertexArray(m_VertexArrayID);
		indexBuffer->BindIndexBuffer();

		m_IndexBuffer = indexBuffer;
	}
}