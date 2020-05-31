#pragma once

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
		using Signature_Ptr_Type = Signature_Type*;

		Signature_Type signature;
		size_t bytes;
		HeapBase* pHeap;
		AllocHeader* pPrev;
		AllocHeader* pNext;
		
		static void Verify(AllocHeader* pHeader);
		static AllocHeader* GetHeaderFromPointer(void* pHeader);
	};

}
