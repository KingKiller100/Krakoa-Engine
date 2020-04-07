#pragma once

#include <cstdint>

namespace krakoa::graphics
{
	class iVertexArray
	{
	public:
		virtual ~iVertexArray();

		virtual void Bind() const noexcept = 0;
		virtual void Unbind() const noexcept = 0;

		static iVertexArray* Create();
	};
}