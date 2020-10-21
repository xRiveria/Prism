#pragma once

#include "Renderer/Buffer.h"

namespace Prism
{
	class VertexArray
	{	
	public:
		virtual ~VertexArray() {}

		virtual void BindVertexArray() const = 0;
		virtual void UnbindVertexArray() const = 0;

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
	
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* CreateVertexArray();
	};

}
