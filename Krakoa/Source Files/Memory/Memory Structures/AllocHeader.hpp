﻿#pragma once

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
	public:
		using Signature_Type = std::uint32_t;

	public:
		USE_RESULT static void* Initialize(AllocHeader* pHeader, const size_t bytes, HeapBase* pHeap) noexcept;
		static AllocHeader* Destroy(void* pData) noexcept;

		
		USE_RESULT static AllocHeader* GetHeaderFromPointer(void* pData);
		USE_RESULT static void* GetPointerFromHeader(AllocHeader* pHeader);
		USE_RESULT static bool VerifyHeader(AllocHeader* pHeader, bool enableAssert = true);
		
	public:
		Signature_Type signature;
		size_t bytes = 0;
		size_t bookmark;
		
#ifndef KRAKOA_RELEASE
		HeapBase* pHeap = nullptr;
		AllocHeader* pPrev = nullptr;
		AllocHeader* pNext = nullptr;
#endif
	};
}
