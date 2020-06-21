#include "Precompile.hpp"
#include "AllocHeader.hpp"

#include "HeapBase.hpp"
#include "MemoryTypes.hpp"

#include "../../Core/Logging/MemoryLogger.hpp"

#include <Maths/BytesUnits.hpp>
#include <Utility/Format/kFormatToString.hpp>


namespace memory
{
	static size_t bookmarkIter = 0;

	void* AllocHeader::Create(AllocHeader* pHeader, const size_t bytes, HeapBase* pHeap) noexcept
	{
		pHeader->signature = KRK_MEMSYSTEM_START_SIG;
		pHeader->bookmark = bookmarkIter++;
		pHeader->pHeap = pHeap;
		pHeader->bytes = bytes;
		pHeader->pPrev = pHeader->pNext = nullptr;
		
		if (pHeader->pHeap->GetPrevAddress())
		{
			auto* pPrevHeader = pHeader->pHeap->GetPrevAddress();
			pHeader->pPrev = pPrevHeader;
			pPrevHeader->pNext = pHeader;
		}
		
		pHeader->pHeap->SetPrevAddress(pHeader);

		auto* pMemEnd = REINTERPRET(Signature_Type*, 
			REINTERPRET(kmaths::Byte_Type*, pHeader)
			+ AllocHeaderSize + bytes);
		*pMemEnd = KRK_MEMSYSTEM_END_SIG;

		pHeap->Allocate(bytes + ControlBlockSize);

		return reinterpret_cast<kmaths::Byte_Type*>(pHeader) + AllocHeaderSize;
	}

	AllocHeader* AllocHeader::Destroy(void* pData) noexcept
	{
		auto* pHeader = GetHeaderFromPointer(pData);

		auto* pHeap = pHeader->pHeap;
		const auto bytes = pHeader->bytes;
		auto*& pPrev = pHeader->pPrev;
		auto*& pNext = pHeader->pNext;

		if (!pPrev && !pNext) // Both null
			pHeap->SetPrevAddress(nullptr);
		else // Maybe one null
		{
			if (pNext)
				pNext->pPrev = pHeader->pPrev;

			if (pPrev)
				pPrev->pNext = pHeader->pNext;

			if (pHeap->GetPrevAddress() == pHeader)
				pHeap->SetPrevAddress(pPrev);

			pPrev = pNext = nullptr;
		}

		pHeap->Deallocate(bytes + ControlBlockSize);

		return pHeader;
	}

	bool AllocHeader::VerifyHeader(AllocHeader* pHeader, bool enableAssert)
	{
		using namespace klib::kFormat;

		if (pHeader->signature != KRK_MEMSYSTEM_START_SIG)
		{
			if (enableAssert)
			{
				MEM_FATAL(false, ToString("CORRUPTED HEAP - Incorrect signature"
					" on a heap - memory Address: {0}\n",
					pHeader));
			}
			return false;
		}

		auto* const pMemEnd = REINTERPRET(Signature_Type*,
			reinterpret_cast<kmaths::Byte_Type*>(pHeader) + AllocHeaderSize + pHeader->bytes);

		if (*pMemEnd != KRK_MEMSYSTEM_END_SIG)
		{
			if (enableAssert)
			{
				MEM_FATAL(false,
					ToString("CORRUPTED HEAP - Incorrect end marker on"
						" a heap - memory address: {0}\n",
						pHeader));
			}
			return false;
		}

		return true; // Both correct
	}

	AllocHeader* AllocHeader::GetHeaderFromPointer(void* pData)
	{
		auto* pHeader = REINTERPRET(AllocHeader*,
			CAST(kmaths::Byte_Type*, pData)
			- AllocHeaderSize);
		
		MEM_ASSERT(VerifyHeader(pHeader));
		return pHeader;
	}

	void* AllocHeader::GetPointerFromHeader(AllocHeader* pHeader)
	{
		if (!VerifyHeader(pHeader, false))
			return nullptr;

		return REINTERPRET(kmaths::Byte_Type*, pHeader)
			+ AllocHeaderSize;
	}
}
