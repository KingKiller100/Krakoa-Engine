#include "Precompile.hpp"
#include "DefaultHeap.hpp"

#include "MemoryTypeSizes.hpp"

#include <Utility/String/kToString.hpp>

namespace memory
{
	std::string GetDefaultHeapStatus(const Heap* pHeap)
	{
		using namespace klib::kString;

		const auto count = pHeap->WalkTheHeap();

		if (!count)
			return "Heap \"Default\" is empty\n";

		std::string report;
		const auto totalBytes = pHeap->GetTotalAllocatedBytes();
		const auto BlockTotal = totalBytes + ControlBlockSize * count;
		size_t index(count);

		report.append(ToString(R"(Heap "Default"
Total Object Bytes: {0}
Total Block Bytes: {1}
Total Number of Allocations: {2}

# Please look below for further details #
)",
totalBytes,
BlockTotal,
count
));

		auto& allocList = *pHeap->GetAllocList<patterns::BiDirectionalLinkedList<AllocHeader>>();

		if (!allocList.head || !allocList.tail)
			return report;

		size_t currentIndex = 0;

		auto* current = allocList.head;
		do {
			const auto& alloc = current->data;
			const auto bytes = alloc.bytes;
			const auto blockBytes = bytes + ControlBlockSize;
			const auto bookmark = alloc.bookmark;
			current = current->next;

			report.append(ToString(R"(
Heap: "Default" 
Index: {0}
Object Bytes: {1}
Block Bytes: {2}
Bookmark: {3}
)",
currentIndex
, bytes
, blockBytes
, bookmark
));


			++currentIndex;
		} while (current != nullptr);

		return report;
	}
}
