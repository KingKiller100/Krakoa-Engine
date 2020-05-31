﻿#include "Precompile.hpp"
#include "HeapBase.hpp"

#include "AllocHeader.hpp"
#include "MemoryTypes.hpp"

#include "../../Core/Logging/MemoryLogger.hpp"

#include <Utility/Format/kFormatToString.hpp>

namespace memory
{
	HeapBase::HeapBase(const char* name) noexcept
		: name(name),
		totalBytes(0),
		pPrevAddress(nullptr),
		vftbl(nullptr)
	{}


	void HeapBase::Initialize(const char* n, Heap_VFTBL* heapVTBL) noexcept
	{
		name = n;
		totalBytes = 0;
		pPrevAddress = nullptr;
		vftbl = heapVTBL;
	}

	void HeapBase::SetName(const char* n) noexcept
	{
		name = n;
	}

	const char* HeapBase::GetName() const noexcept
	{
		return name;
	}

	void HeapBase::Allocate(const size_t bytes) noexcept
	{
		totalBytes += bytes;
	}

	void HeapBase::Deallocate(const size_t bytes) noexcept
	{
		totalBytes -= bytes;
	}

	size_t HeapBase::GetTotalAllocatedBytes() const noexcept
	{
		return totalBytes;
	}

	size_t HeapBase::WalkTheHeap() const
	{
		auto* pCurrentHeader = pPrevAddress; // casts to AllocHeader to find previous and next

		if (!pCurrentHeader)
			return 0;

		if (!pCurrentHeader->pPrev)
			return 1;

		unsigned count(1);
		pCurrentHeader = pCurrentHeader->pPrev;

		while (pCurrentHeader && pCurrentHeader->pNext != pCurrentHeader)
		{
			MEM_FATAL(pCurrentHeader->signature == KRK_MEMSYSTEM_SIGNATURE,
				klib::kFormat::ToString("CORRUPTED HEAP - Incorrect signature on heap: \"{0}\" position: {1}\n",
					pCurrentHeader->pHeap->name,
					count));

			auto* pMemEnd = REINTERPRET(AllocHeader::Signature_Ptr_Type, REINTERPRET(Byte_Ptr_Type, pCurrentHeader) + AllocHeaderBytes + pCurrentHeader->bytes);

			MEM_FATAL(*pMemEnd == KRK_MEMSYSTEM_ENDMARKER,
				klib::kFormat::ToString("CORRUPTED HEAP - Incorrect end marker on heap: \"{0}\" position: {1}\n",
					pCurrentHeader->pHeap->name,
					count));

			pCurrentHeader = pCurrentHeader->pPrev;

			count++;
		}

		return count;
	}

	void HeapBase::SetPrevAddress(AllocHeader* prev) noexcept
	{
		pPrevAddress = prev;
	}

	AllocHeader* HeapBase::GetPrevAddress() const noexcept
	{
		return pPrevAddress;
	}

	std::string HeapBase::GetStatus() const
	{
		MEM_FATAL(vftbl->getStatusFunc, "HeapBase's vftbl is unset");
		return vftbl->getStatusFunc(this);
	}
}
