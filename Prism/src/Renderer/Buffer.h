#pragma once
#include "PrismPrecompiledHeader.h"
//Remember that this is a pure VTable class meant to be inherited by other graphics APIs.

namespace Prism
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}
		
		virtual void BindVertexBuffer() const = 0;
		virtual void UnbindVertexBuffer() const = 0;

		//We could create a constructor. However, if we do so, we can't actually choose which type of API we want.
		//This create function will decide which API our renderer is using and thus which derived graphics class we instantiate and return.
		static VertexBuffer* CreateVertexBuffer(float* vertices, uint32_t size);

	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void BindIndexBuffer() const = 0;
		virtual void UnbindIndexBuffer() const = 0;

		virtual uint32_t GetIndicesCount() const = 0;

		static IndexBuffer* CreateIndexBuffer(uint32_t* indices, uint32_t size);
	};
}
