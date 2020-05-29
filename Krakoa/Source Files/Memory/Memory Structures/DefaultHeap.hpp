#pragma once

#include "HeapBase.hpp"

#include <string>

namespace memory
{
	class DefaultHeap final : public HeapBase
	{
	public:
		DefaultHeap() noexcept;
		~DefaultHeap() noexcept;
	};
}