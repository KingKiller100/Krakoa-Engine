#pragma once

#include "MemorySignatures.hpp"

#include <HelperMacros.hpp>

namespace memory
{
	class HeapBase;

	struct AllocHeader
	{
	public:
		USE_RESULT static void* Create(AllocHeader* pHeader, const size_t bytes, HeapBase* pHeap) noexcept;
		USE_RESULT static AllocHeader* Destroy(void* pData) noexcept;

		USE_RESULT static AllocHeader* GetHeaderFromPointer(void* pData);
		USE_RESULT static void* GetPointerFromHeader(AllocHeader* pHeader);
		USE_RESULT static bool VerifyHeader(AllocHeader* pHeader, bool enableAssert = true);
		
	public:
		Signature_Type signature;
		size_t bytes;
		size_t bookmark;
		
#ifndef KRAKOA_RELEASE
		HeapBase* pHeap = nullptr;
		AllocHeader* pPrev = nullptr;
		AllocHeader* pNext = nullptr;
#endif
	};
}
