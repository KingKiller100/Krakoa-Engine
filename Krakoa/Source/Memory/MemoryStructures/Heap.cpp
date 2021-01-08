#include "Precompile.hpp"
#include "Heap.hpp"

#include "AllocHeader.hpp"

#include "../MemoryDebug.hpp"
#include "../../Logging/MemoryLogger.hpp"

#include <Maths/BytesUnits.hpp>

namespace memory
{
	size_t Heap::s_TotalLifetimeAllocations = 0;
	
	void Heap::Initialize(const char* n, Heap_VFTBL* heapVTBL) noexcept
	{
		name = n;
		totalBytes = 0;
		pPrevAddress = nullptr;
		vftbl = heapVTBL;

		family.pParent = nullptr;
		family.pFirstChild = nullptr;
		family.pPrevSibling = nullptr;
		family.pNextSibling = nullptr;
	}

	bool Heap::AddToParent(Heap* pParent)
	{
		return false;
	}

	void Heap::SetName(const char* n) noexcept
	{
		name = n;
	}

	const char* Heap::GetName() const noexcept
	{
		return name;
	}

	const Heap::Family& Heap::GetFamily() const noexcept
	{
		return family;
	}

	Heap::Family& Heap::GetFamily() noexcept
	{
		return family;
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

	size_t Heap::GetLastBookmark() const
	{
		return pPrevAddress->bookmark;
	}

	void Heap::DeleteLeaks()
	{
		auto& pCurrentHeader = pPrevAddress;

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
		MEM_ASSERT(vftbl->getStatusFunc != nullptr,
			"Heap's vftbl is empty");
		return vftbl->getStatusFunc(this);
	}
}
