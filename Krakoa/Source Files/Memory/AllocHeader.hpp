#pragma once

// 3735929054
#define KRK_MEMSYSTEM_SIGNATURE 0xdeadc0de 

// 3735928559
#define KRK_MEMSYSTEM_ENDMARKER 0xdeadbeef

namespace memory
{
	class Heap;
	
	struct AllocHeader
	{
		using Signature_Type = unsigned;
		using Signature_Ptr_Type = Signature_Type*;
		
		Signature_Type signature;
		Heap* pHeap;
		size_t bytes;
		AllocHeader* pPrev;
		AllocHeader* pNext;
	};
}
