#pragma once

#include "MemoryLinkedList.hpp"

#include <HelperMacros.hpp>

namespace memory
{
	class Heap;

	struct AllocHeader : MemoryLinkedList<AllocHeader>
	{
	public:
		USE_RESULT static void* Create(AllocHeader* pHeader, const size_t bytes, Heap* pHeap) noexcept;
		USE_RESULT static AllocHeader* Destroy(void* pData) noexcept;

		USE_RESULT static AllocHeader* GetHeaderFromPointer(void* pData);
		USE_RESULT static void* GetPointerFromHeader(AllocHeader* pHeader);
		USE_RESULT static bool VerifyHeader(AllocHeader* pHeader, bool enableAssert = true);

	public:
		size_t bookmark;
		Heap* pHeap = nullptr;
	};
}
