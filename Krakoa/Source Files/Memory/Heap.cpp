#include "Precompile.hpp"
#include "Heap.hpp"

#include "AllocHeader.hpp"
#include "MemoryTypes.hpp"

#include "../Core/Logging/CoreLogger.hpp"

#include <Utility/Format/kFormatToString.hpp>

#include <Utility/Debug Helper/Exceptions/NotImplementedException.hpp>

namespace memory
{
	Heap::Heap(const char* name) noexcept
		: name(name),
		totalBytes(0),
		pPrevAddress(nullptr)
	{}

	Heap::~Heap() noexcept
		= default;

	void Heap::Initialize(const char* n) noexcept
	{
		name = n;
		totalBytes = 0;
		pPrevAddress = nullptr;
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

	std::string Heap::WalkHeap() const
	{
		size_t totalHeapBytes(0);
		unsigned int count(0);

		std::string details;

		if (!pPrevAddress) // Gets prev address on the heap
		{
			details.append(klib::kFormat::ToString(
				"Heap data \"{0}\" total bytes: {1}\n",
				name,
				(totalBytes)
			));
			
			details.append(klib::kFormat::ToString(
				"Heap \"{0}\" total bytes (including AllocHeader and signature): {1}\n",
				name,
				(totalBytes + allocHeaderBytes + signatureBytes)
			));
			
			return details;
		}

		auto* pCurrentHeader = static_cast<AllocHeader*>(pPrevAddress); // casts to AllocHeader to find previous and next

		if (pCurrentHeader)
		{
			size_t blockBytes(0);

			if (pCurrentHeader->pPrev)
			{
				while (pCurrentHeader->pPrev && pCurrentHeader->pPrev != pCurrentHeader)
				{
					pCurrentHeader = pCurrentHeader->pPrev;
				}

				while (pCurrentHeader && pCurrentHeader->pPrev != pCurrentHeader)
				{
					details.append(klib::kFormat::ToString(
						"{0} {1}: size for each class allocated on the heap (excluding AllocHeader and signature) : {2}\n",
						pCurrentHeader->pHeap->GetName(),
						count,
						pCurrentHeader->bytes
					));

					blockBytes = allocHeaderBytes + pCurrentHeader->bytes + signatureBytes;
					count++;

					KRK_FATAL(pCurrentHeader->signature == KRK_MEMSYSTEM_SIGNATURE,
						klib::kFormat::ToString("CORRUPTED HEAP - Incorrect signature on heap: \"{0}\" position: {1}\n",
							pCurrentHeader->pHeap->name,
							count));

					auto* pMemEnd = REINTERPRET(AllocHeader::Signature_Ptr_Type, REINTERPRET(Byte_Ptr_Type, pCurrentHeader) + allocHeaderBytes + pCurrentHeader->bytes);

					KRK_FATAL(*pMemEnd == KRK_MEMSYSTEM_ENDMARKER,
						klib::kFormat::ToString("CORRUPTED HEAP - Incorrect end marker on heap: \"{0}\" position: {1}\n",
							pCurrentHeader->pHeap->name,
							count));

					pCurrentHeader = pCurrentHeader->pNext;
					totalHeapBytes += totalBytes;
				}
			}
			else
			{
				blockBytes = allocHeaderBytes + pCurrentHeader->bytes + signatureBytes;
				totalHeapBytes += totalBytes;

				details.append(klib::kFormat::ToString(
					"{0} {1}: size for each class allocated on the heap(excluding AllocHeader) : {2}\n",
					pCurrentHeader->pHeap->GetName(),
					count,
					blockBytes
				));
			}
		}


		if (totalHeapBytes)
			details.append(klib::kFormat::ToString(
				R"(Heap "{0}" total bytes (including AllocHeader and signature: {1}\n)",
				name,
				totalHeapBytes
			));
		else
			details.append(klib::kFormat::ToString("Heap {0} is empty\n", name));

		return details;
	}

	void* Heap::GetPrevAddress() const noexcept
	{
		return pPrevAddress;
	}

	void Heap::SetPrevAddress(void* prev) noexcept
	{
		pPrevAddress = prev;
	}
}
