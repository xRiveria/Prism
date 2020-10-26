#pragma once

#include "Prism/Renderer/Buffer.h"

namespace Prism
{
	class VertexArray
	{	
	public:
		virtual ~VertexArray() {}

		virtual void BindVertexArray() const = 0;
		virtual void UnbindVertexArray() const = 0;

		virtual void AddVertexBuffer(Reference<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(Reference<IndexBuffer>& indexBuffer) = 0;
	
		virtual const std::vector<Reference<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Reference<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* CreateVertexArray();
	};

}
