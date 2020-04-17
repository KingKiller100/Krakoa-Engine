#pragma once

#include "iBuffers.hpp"

#include <cstdint>

namespace krakoa::graphics
{
	class iVertexArray
	{
	public:
		virtual ~iVertexArray();

		virtual void Bind() const noexcept = 0;
		virtual void Unbind() const noexcept = 0;

		virtual void AddVertexBuffer(iVertexBuffer* vertexBuffer) = 0;
		virtual void SetIndexBuffer(iIndexBuffer* indexBuffer) noexcept = 0;

		virtual const std::vector<std::shared_ptr<iVertexBuffer>>& GetVertexBuffers() const noexcept = 0;
		virtual const std::unique_ptr<iIndexBuffer>& GetIndexBuffer() const noexcept = 0;

		static iVertexArray* Create();
	};
}