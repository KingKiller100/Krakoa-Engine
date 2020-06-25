#pragma once

#include <HelperMacros.hpp>

#include "MemorySignatures.hpp"

namespace memory
{
	template<typename T>
	class MemoryLinkedList // Doubly linked list of pointers
	{
		using Type = T;
		
	public:
		static void CreateLinkedList(MemoryLinkedList* pLinkedList, const size_t bytes, T* pPrev, T* pNext) noexcept
		{
			pLinkedList->signature = KRK_MEMSYSTEM_START_SIG;
			pLinkedList->bytes = bytes;
			pLinkedList->pPrev = pPrev;
			pLinkedList->pNext = pNext;
		}

		USE_RESULT static bool VerifyLinkedList(MemoryLinkedList* const pLinkedList) noexcept
		{
			return (pLinkedList->signature == KRK_MEMSYSTEM_START_SIG);
		}
		
	public:
		Signature_Type signature;
		size_t bytes;
		T* pPrev;
		T* pNext;
	};
}
