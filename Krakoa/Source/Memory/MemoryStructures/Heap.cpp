#include "Precompile.hpp"
#include "Heap.hpp"

#include "AllocHeader.hpp"

#include "../MemoryDebug.hpp"

#include "../../Util/Fmt.hpp"
#include "../MemoryUtil.hpp"
#include <Utility/Debug/kDebugger.hpp>

namespace memory
{
	size_t Heap::s_TotalLifetimeBytesAllocated = 0;

	using namespace patterns;

	void Heap::Initialize(const char* n, Heap_VFTBL* heapVTBL) noexcept
	{
		name = n;
		totalBytes = 0;
		allocListVoid = nullptr;
		vftbl = heapVTBL;
		allocListVoid = Allocate<BiDirectionalLinkedList<AllocHeader>>();
		allocationsCount = 0;
		// family.pParent = nullptr;
		// family.pFirstChild = nullptr;
		// family.pPrevSibling = nullptr;
		// family.pNextSibling = nullptr;
	}

	void Heap::ShutDown()
	{
		const auto leaks = WalkTheHeap();
		
		MEM_ASSERT(leaks == 0,
			util::Fmt("{0} has {1} remaining allocation(s)", name, leaks));
		DeleteLeaks();

		Deallocate(allocListVoid);
		allocListVoid = nullptr;
		allocationsCount = 0;
	}

	// bool Heap::AddToParent(Heap* pParent)
	// {
		// return false;
	// }

	void Heap::SetName(const char* n) noexcept
	{
		name = n;
	}

	const char* Heap::GetName() const noexcept
	{
		return name;
	}

	void Heap::IncrementAllocationsCount()
	{
		++allocationsCount;
	}

	void Heap::DecrementAllocationsCount()
	{
		MEM_ASSERT(allocationsCount > 0, "Decrementing allocation count when no allocations exist");
		--allocationsCount;
	}

	size_t Heap::GetCount() const
	{
		return allocationsCount;
	}

	// const Heap::Family& Heap::GetFamily() const noexcept
	// {
		// return family;
	// }

	// Heap::Family& Heap::GetFamily() noexcept
	// {
		// return family;
	// }

	void Heap::AllocateBytes(const size_t bytes) noexcept
	{
		totalBytes += bytes;
	}

	void Heap::DeallocateBytes(const size_t bytes) noexcept
	{
		totalBytes -= bytes;
	}

	size_t Heap::GetTotalAllocatedBytes() const noexcept
	{
		return totalBytes;
	}

	size_t Heap::WalkTheHeap() const
	{
		auto [head, tail] = *static_cast<AllocHeaderLinkedList*>(allocListVoid);
		
		if (!head)
			return 0;
		
		auto* current = head;

		while (current != tail)
		{
			if (current == nullptr)
				break;
			
			current->data.VerifyHeader(true);
			current = current->next;
		}

		return allocationsCount;
	}

	void Heap::DeleteLeaks() const
	{
		auto* allocList = static_cast<BiDirectionalLinkedList<AllocHeader>*>(allocListVoid);
		if (!allocList->head)
			return;

		while (allocList->tail != allocList->head)
		{
			const auto toDelete = allocList->tail;
			allocList->tail = allocList->tail->prev;
			Deallocate(toDelete);
		}
		
		Deallocate(allocList->head);
		allocList->head = allocList->tail = nullptr;
	}

	size_t Heap::GetLastBookmark() const
	{
		auto* allocList = static_cast<BiDirectionalLinkedList<AllocHeader>*>(allocListVoid);
		if (allocList->tail)
			return allocList->tail->data.bookmark;

		return 0;
	}
	
	std::string Heap::GetStatus() const
	{
		MEM_ASSERT(vftbl->getStatusFunc != nullptr,
			"Heap's vftbl is empty");
		return vftbl->getStatusFunc(this);
	}
}
