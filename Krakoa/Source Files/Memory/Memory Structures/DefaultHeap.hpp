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

	static void DefaultCallObjectDestructor(void* pMemPtr)
	{
		//No Impl
	}

	static std::string GetDefaultHeapStatus(const HeapBase* pHeap)
	{
		using namespace klib::kFormat;

		std::string report = "Heap \"Default\" is empty\n";
		const auto totalBytes = pHeap->GetTotalAllocatedBytes();


		auto* pCurrentHeader = static_cast<AllocHeader*>(pHeap->GetPrevAddress()); // casts to AllocHeader to find previous and next

		if (!pCurrentHeader || !pCurrentHeader->pPrev)
			return report;

		report.clear();

		unsigned index(0);

		while (pCurrentHeader && pCurrentHeader->pNext != pCurrentHeader)
		{
			MEM_FATAL(pCurrentHeader->signature == KRK_MEMSYSTEM_SIGNATURE,
				klib::kFormat::ToString("CORRUPTED HEAP - Incorrect signature on heap: \"{0}\" position: {1}\n",
					pCurrentHeader->pHeap->GetName(),
					index));

			auto* pMemEnd = REINTERPRET(AllocHeader::Signature_Ptr_Type, REINTERPRET(Byte_Ptr_Type, pCurrentHeader) + AllocHeaderBytes + pCurrentHeader->bytes);

			MEM_FATAL(*pMemEnd == KRK_MEMSYSTEM_ENDMARKER,
				klib::kFormat::ToString("CORRUPTED HEAP - Incorrect end marker on heap: \"{0}\" position: {1}\n",
					pCurrentHeader->pHeap->GetName(),
					index));

			const auto blockBytes = pCurrentHeader->bytes + MemoryPaddingBytes;

			report.append(ToString(R"(
Heap: "Default" Index: {0}
Object Bytes: {1}
Block Bytes: {2})",
index,
pCurrentHeader->bytes,
blockBytes));

			++index;

			pCurrentHeader = pCurrentHeader->pPrev;
		}

		return report;
	}
}
