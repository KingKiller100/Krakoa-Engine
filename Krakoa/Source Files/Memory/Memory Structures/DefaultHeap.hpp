#pragma once

#include "HeapBase.hpp"

#include <string>

#include "AllocHeader.hpp"
#include "MemoryTypes.hpp"
#include "../../Core/Logging/MemoryLogger.hpp"

#include <Utility/Format/kFormatToString.hpp>

namespace memory
{
	class DefaultHeap final : public HeapBase
	{
	public:
		DefaultHeap() noexcept;
		~DefaultHeap() noexcept;
	};

	static std::string GetDefaultHeapStatus(const HeapBase* pHeap)
	{
		using namespace klib::kFormat;

		std::string report = "Heap \"Default\" is empty\n";
		
		const auto totalBytes = pHeap->GetTotalAllocatedBytes();
		const auto numOfAllocs = pHeap->WalkTheHeap();
		const auto BlockTotal = totalBytes + MemoryPaddingBytes * numOfAllocs;
		size_t index(numOfAllocs);

		report.append(ToString(R"(
Total Bytes: {0}
Total Bytes (Including Padding): {1}
Total Number of Allocations: {2}

# Please look below for further details #
)",
totalBytes,
BlockTotal,
numOfAllocs
));

		auto* pCurrentHeader = static_cast<AllocHeader*>(pHeap->GetPrevAddress()); // casts to AllocHeader to find previous and next

		if (!pCurrentHeader || !pCurrentHeader->pPrev)
			return report;

		report.clear();


		while (pCurrentHeader->pPrev && pCurrentHeader != pCurrentHeader->pPrev) // Move to the first 
		{
			pCurrentHeader = pCurrentHeader->pPrev;
		}

		do {
			const auto bytes = pCurrentHeader->bytes;
			const auto blockBytes = pCurrentHeader->bytes + MemoryPaddingBytes;

			report.append(ToString(R"(
Heap: "Default" Index: {0}
Object Bytes: {1}
Block Bytes: {2}
)",
index--,
bytes,
blockBytes));

			pCurrentHeader = pCurrentHeader->pNext;
		} while (index);


		
		report.append(ToString(R"(
Heap: "Default" Index: {0}
Object Bytes: {1}
Block Bytes: {2}
)",
index,
pCurrentHeader->bytes,
pCurrentHeader->bytes + MemoryPaddingBytes));

		return report;
	}
}
