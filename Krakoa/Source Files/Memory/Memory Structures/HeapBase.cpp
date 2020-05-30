#include "Precompile.hpp"
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
		vtbl(nullptr)
	{}


	void HeapBase::Initialize(const char* n, Heap_VTBL* heapVTBL) noexcept
	{
		name = n;
		totalBytes = 0;
		pPrevAddress = nullptr;
		vtbl = heapVTBL;
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

	size_t HeapBase::WalkTheHeap() const noexcept
	{
		auto* pCurrentHeader = static_cast<AllocHeader*>(pPrevAddress); // casts to AllocHeader to find previous and next

		if (!pCurrentHeader || !pCurrentHeader->pPrev)
			return 0;

		unsigned count(0);

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

			count++;

			pCurrentHeader = pCurrentHeader->pPrev;
		}

		return count;
	}

	void HeapBase::SetPrevAddress(void* prev) noexcept
	{
		pPrevAddress = prev;
	}

	void* HeapBase::GetPrevAddress() const noexcept
	{
		return pPrevAddress;
	}

	std::string HeapBase::GetStatus() const noexcept
	{
		MEM_FATAL(vtbl->getStatusFunc, "HeapBase's vtbl is unset");
		return vtbl->getStatusFunc(this);
	}

	void HeapBase::CallObjectDestructor(void* pMemPtr) const noexcept
	{
		MEM_FATAL(vtbl->callObjFunc, "HeapBase's vtbl is unset");
		vtbl->callObjFunc(pMemPtr);
	}
}
