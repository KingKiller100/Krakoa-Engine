#pragma once

#include <HelperMacros.hpp>

// 3735929054
#define KRK_MEMSYSTEM_START_SIG 0xdeadc0de 

// 3735928559
#define KRK_MEMSYSTEM_END_SIG 0xdeadbeef

namespace memory
{
	class HeapBase;

	struct AllocHeader
	{
		using Signature_Type = unsigned;

		Signature_Type signature;
		size_t bookmark;
		size_t bytes;
		HeapBase* pHeap = nullptr;
		AllocHeader* pPrev = nullptr;
		AllocHeader* pNext = nullptr;

		USE_RESULT size_t GetMemoryBookmark() const noexcept;
		
		USE_RESULT static bool VerifyHeader(AllocHeader* pHeader, bool enableAssert = true);
		USE_RESULT static AllocHeader* GetHeaderFromPointer(void* pData);
	};
}
