#include "Precompile.hpp"
#include "AllocHeader.hpp"

#include "Heap.hpp"
#include "MemoryTypeSizes.hpp"

#include "../MemoryDebug.hpp"

#include "../../Logging/MemoryLogger.hpp"

#include <Maths/BytesUnits.hpp>

namespace memory
{
	namespace
	{
		size_t g_BookmarkCount = 0;
	}
	

	void* CreateNode(AllocHeaderLinkedList::Node_t* node, size_t bytes, Heap* heap)
	{
		std::memset(node, 0,  ControlBlockSize + bytes);
		auto& header = node->data;
		auto& allocList = *heap->GetAllocList<patterns::BiDirectionalLinkedList<AllocHeader>>();
		
		header.Create(heap, bytes, g_BookmarkCount++);

		if (allocList.head == nullptr && allocList.tail == nullptr)
		{
			allocList.head = allocList.tail = node;
		}
		else if (allocList.head && !allocList.tail)
		{
			MEM_FATAL("AllocHeaderLinkedList head is set but tail is null");
		}
		else if (!allocList.head && allocList.tail)
		{
			MEM_FATAL("AllocHeaderLinkedList tail is set but head is null");
		}
		else
		{
			auto* prev = allocList.tail;
			prev->next = node;
			node->prev = prev;
			allocList.tail = node;
		}

		return reinterpret_cast<std::byte*>(node) + NodeSize;
	}

	void AllocHeader::Create(Heap* heap, const size_t bytes, size_t bookmark) noexcept
	{
		this->signature = MemoryBlockSignatureStart;
		this->pHeap = heap;
		this->bytes = bytes;
		this->bookmark = bookmark;
		
		auto* pMemEnd = reinterpret_cast<Signature_Type*>(
			reinterpret_cast<std::byte*>(this) + NodeSize + bytes
			);
		*pMemEnd = MemoryBlockSignatureEnd;

		heap->Allocate(bytes + ControlBlockSize);
	}

	void DestroyNode(AllocHeaderNode* node)
	{
		auto& header = node->data;
		auto* heap = header.pHeap;
		auto& [head, tail] = *heap->GetAllocList<patterns::BiDirectionalLinkedList<AllocHeader>>();
		
		if (head == tail)
		{
			head = tail = nullptr;
		}
		else if (head && !tail)
		{
			MEM_FATAL("AllocHeaderLinkedList head is set but tail is null");
		}
		else if (!head && tail)
		{
			MEM_FATAL("AllocHeaderLinkedList tail is set but head is null");
		}
		else
		{
			if (node->next)
			{
				node->next->prev = node->prev;
			}

			if (node->prev)
			{
				node->prev->next = node->next;
			}
		}

		header.Destroy();
	}

	size_t GetTotalAllocationsCount() noexcept
	{
		return g_BookmarkCount;
	}

	void AllocHeader::Destroy() noexcept
	{
		VerifyHeader(true);

		this->pHeap->Deallocate(bytes + ControlBlockSize);
	}


	AllocHeaderNode* GetNodeFromDataPointer(void* ptr)
	{
		return reinterpret_cast<AllocHeaderNode*>(static_cast<std::byte*>(ptr) - NodeSize);
	}

	void* GetDataPointerFromNode(AllocHeaderNode* node)
	{
		if (!node)
			return nullptr;
		
		if (!node->data.VerifyHeader(false))
			return nullptr;

		return reinterpret_cast<std::byte*>(node) + NodeSize;
	}

	bool AllocHeader::VerifyHeader(bool enableAssert)
	{
		using namespace klib::kString;

		if (signature != MemoryBlockSignatureStart)
		{
			if (enableAssert)
			{
				MEM_FATAL("CORRUPTED HEAP - Incorrect start signature"
					" on a heap\n");
			}
			return false;
		}

		auto* const pMemEnd =
			reinterpret_cast<Signature_Type*>(
				reinterpret_cast<std::byte*>(this) + NodeSize + bytes
				);

		if (*pMemEnd != MemoryBlockSignatureEnd)
		{
			if (enableAssert)
			{
				MEM_FATAL("CORRUPTED HEAP - Incorrect end marker on"
					" a heap - memory address: {0}\n");
			}
			return false;
		}

		return true; // Both correct
	}
}
