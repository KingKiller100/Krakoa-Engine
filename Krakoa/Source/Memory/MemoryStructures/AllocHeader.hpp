#pragma once

#include "../../Patterns/LinkedList.hpp"

#include <HelperMacros.hpp>

namespace memory
{
	class Heap;

	struct AllocHeader
	{
		using Signature_Type = std::uint32_t;
		// 3735929054
		static constexpr Signature_Type MemoryBlockSignatureStart = 0xdeadc0de;

		// 3735928559
		static constexpr Signature_Type MemoryBlockSignatureEnd = 0xdeadbeef;
	public:
		AllocHeader() noexcept = default;
		AllocHeader(AllocHeader&&) noexcept = default;
		AllocHeader& operator=(AllocHeader&&) noexcept = default;
		
		AllocHeader(const AllocHeader&) = delete;
		AllocHeader& operator=(const AllocHeader&) const = delete;
		
		void Create(Heap* heap, const size_t bytes, size_t bookmark) noexcept;
		void Destroy() noexcept;
		bool VerifyHeader(bool enableAssert = false);

	public:
		std::uint32_t signature;
		size_t bookmark;
		size_t bytes;
		Heap* pHeap = nullptr;
	};
	
	using AllocHeaderLinkedList = patterns::BiDirectionalLinkedList<AllocHeader>;
	using AllocHeaderNode = AllocHeaderLinkedList::Node_t;
	
	void* CreateNode(AllocHeaderLinkedList::Node_t* node, size_t bytes, Heap* heap);
	AllocHeaderNode* GetNodeFromDataPointer(void* ptr);
	void* GetDataPointerFromNode(AllocHeaderNode* allocNode);
	void DestroyNode(AllocHeaderNode* node);

	size_t GetTotalAllocationsCount() noexcept;
}
