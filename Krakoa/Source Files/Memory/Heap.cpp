#include "Precompile.hpp"
#include "Heap.hpp"

#include "AllocHeader.hpp"
#include "MemoryTypes.hpp"

#include "../Core/Logging/MemoryLogger.hpp"

#include <Utility/Format/kFormatToString.hpp>


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

	std::string Heap::GetStatus() const
	{
		using namespace klib::kFormat;

		const auto count = WalkTheHeap();
		const auto multiplier = (count ? count : 1);

		const auto bytesPerBlock = (totalBytes / multiplier);
		const auto bytesPer = bytesPerBlock - MemoryPaddingBytes;
		const auto totalBytesOfThisObject = bytesPer * multiplier;

		std::string details;

		if (count)
		{
			details.append(ToString(
				R"(Heap "{0}"
Count: {1}
Bytes per object: {2}
Bytes per block: {3}
Object's Total Bytes In Memory: {4}
Heap's Total Bytes In Memory: {5}
)",
name,
count,
bytesPer,
bytesPerBlock,
totalBytesOfThisObject,
totalBytes));
		}
		else if (totalBytes)
		{
			details.append(ToString(
				R"(Heap "{0}"
Count: 1
Bytes per object: {1}
Bytes per block: {2}
Total Object Bytes In Memory: {1}
Total Block Bytes In Memory: {2}
)",
name,
bytesPer,
totalBytes));
		}
		else
			details.append(ToString("Heap \"{0}\" is empty\n", name));


		return details;
	}

	size_t Heap::WalkTheHeap() const noexcept
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

	void* Heap::GetPrevAddress() const noexcept
	{
		return pPrevAddress;
	}

	void Heap::SetPrevAddress(void* prev) noexcept
	{
		pPrevAddress = prev;
	}
}
