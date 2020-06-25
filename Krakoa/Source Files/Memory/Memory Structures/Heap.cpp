#include "Precompile.hpp"
#include "Heap.hpp"

#include "AllocHeader.hpp"

#include "../../Core/Logging/MemoryLogger.hpp"

#include <Maths/BytesUnits.hpp>

namespace memory
{
	Heap::Heap(const char* name) noexcept
		: name(name),
		totalBytes(0),
		pPrevAddress(nullptr),
		vftbl(nullptr)
	{}


	void Heap::Initialize(const char* n, Heap_VFTBL* heapVTBL) noexcept
	{
		name = n;
		totalBytes = 0;
		pPrevAddress = nullptr;
		vftbl = heapVTBL;
	}

	void Heap::SetName(const char* n) noexcept
	{
		name = n;
	}

	const char* Heap::GetName() const noexcept
	{
		return name;
	}

	void Heap::Allocate(const size_t bytes) noexcept
	{
		totalBytes += bytes;
	}

	void Heap::Deallocate(const size_t bytes) noexcept
	{
		totalBytes -= bytes;
	}

	size_t Heap::GetTotalAllocatedBytes() const noexcept
	{
		return totalBytes;
	}

	size_t Heap::WalkTheHeap() const
	{
		auto* pCurrentHeader = pPrevAddress; // casts to AllocHeader to find previous and next

		if (!pCurrentHeader)
			return 0;

		if (!pCurrentHeader->pPrev)
			return 1;

		unsigned count(1);
		pCurrentHeader = pCurrentHeader->pPrev;

		while (pCurrentHeader 
			&& pCurrentHeader->pNext != pCurrentHeader 
			&& AllocHeader::VerifyHeader(pCurrentHeader))
		{
			pCurrentHeader = pCurrentHeader->pPrev;
			count++;
		}

		return count;
	}

	void Heap::DeleteLeaks()
	{
		auto& pCurrentHeader = pPrevAddress; // casts to AllocHeader to find previous and next

		if (!pCurrentHeader)
			return;

		while (pCurrentHeader 
			&& pCurrentHeader->pPrev != pCurrentHeader 
			&& AllocHeader::VerifyHeader(pCurrentHeader))
		{
			auto* pPrev = pCurrentHeader->pPrev;
			auto* ptr = static_cast<kmaths::Byte_Type*>(AllocHeader::GetPointerFromHeader(pCurrentHeader));
			delete ptr;
			pCurrentHeader = pPrev;
		}
	}

	void Heap::SetPrevAddress(AllocHeader* prev) noexcept
	{
		pPrevAddress = prev;
	}

	AllocHeader* Heap::GetPrevAddress() const noexcept
	{
		return pPrevAddress;
	}

	std::string Heap::GetStatus() const
	{
		MEM_FATAL(vftbl->getStatusFunc, "Heap's vftbl is empty");
		return vftbl->getStatusFunc(this);
	}
}
