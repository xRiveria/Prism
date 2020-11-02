#pragma once
#include "PrismPrecompiledHeader.h"
//Remember that this is a pure VTable class meant to be inherited by other graphics APIs.

namespace Prism
{
	enum class ShaderDataType 
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4,
		Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}

		PRISM_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string elementName;
		ShaderDataType elementType;
		uint32_t elementSize;
		uint32_t elementOffset;
		bool elementNormalized;

		BufferElement() {}
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : elementType(type), elementName(name), elementSize(ShaderDataTypeSize(type)), elementOffset(0), elementNormalized(normalized)
		{			
		}

		uint32_t GetComponentCount() const
		{
			switch (elementType)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:	    return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 3 * 4;
				case ShaderDataType::Bool:		return 1;
			}

			PRISM_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:  
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		inline const uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.elementOffset = offset; //Offset is the spacing between each element type.
				offset += element.elementSize;
				m_Stride += element.elementSize; //Stride is the spacing between each Vertex. 
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;
		
		virtual void BindVertexBuffer() const = 0;
		virtual void UnbindVertexBuffer() const = 0;

		virtual void SetBufferLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		//We could create a constructor. However, if we do so, we can't actually choose which type of API we want.
		//This create function will decide which API our renderer is using and thus which derived graphics class we instantiate and return.
		static Reference<VertexBuffer> CreateVertexBuffer(uint32_t size);
		static Reference<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size);
	};

	//Currently, Prism only supports 32-bit Index Buffers.
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void BindIndexBuffer() const = 0;
		virtual void UnbindIndexBuffer() const = 0;

		virtual uint32_t GetIndicesCount() const = 0;

		static Reference<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count);
	};
}
