#include "Precompile.hpp"
#include "HeapFactory.hpp"

#include "DefaultHeap.hpp"

#include "../MemoryPool.hpp"
#include "../../Core/Logging/MemoryLogger.hpp"

#include <Maths/Constants.hpp>
#include <Maths/BytesTypes.hpp>
#include <Maths/kAlgorithms.hpp>

#include <Utility/File System/kFileSystem.hpp>
#include <Utility/String/kStringManipulation.hpp>

#include <iostream>

namespace memory
{
	HeapBase* HeapFactory::defaultHeap = nullptr;
	HeapFactory::HeapList HeapFactory::heaps{};

	void HeapFactory::Initialize() noexcept
	{
		using namespace klib;

		const auto path = kFileSystem::GetExeDirectory() + "Logs\\Memory.log";

		if (kFileSystem::CheckFileExists(path))
		{
			kFileSystem::RemoveFile(path);
		}

		MEM_INIT_LOGS();
		MEM_SET_FLUSHING(true);
		MEM_NORM(R"(Definitions:
Object - "Structured piece of data in memory"
Block -  "Allocated space of an object in memory including AllocHeader & end marker"
)")

#ifdef KRAKOA_RELEASE
MEM_TOGGLE_LOGGING(); // Disable memory logging
#endif
	}

	void HeapFactory::ShutDown() noexcept
	{
		size_t totalBytes(0);
		size_t totalAllocations(0);

		for (auto i = 1; i < heaps.size() && heaps[i] != nullptr; ++i)
		{
			auto& heap = heaps[i];

			totalBytes += heap->GetTotalAllocatedBytes();
			totalAllocations += heap->WalkTheHeap();
			MEM_INFO(heap->GetStatus());

			heap->DeleteLeaks();
			free(heap);
			heap = nullptr;
		}

		totalBytes += defaultHeap->GetTotalAllocatedBytes();
		totalAllocations += defaultHeap->WalkTheHeap();
		MEM_INFO(defaultHeap->GetStatus());

		LogTotalBytes(&totalBytes);
		LogTotalAllocations(&totalAllocations);
	}

	HeapBase* HeapFactory::GetDefaultHeap() noexcept
	{
		static Heap_VFTBL localVFTBL(GetDefaultHeapStatus);

		if (!defaultHeap)
		{
			defaultHeap = static_cast<HeapBase*>(malloc(sizeof(HeapBase)));
			defaultHeap->Initialize("Default", &localVFTBL);
			heaps.front() = defaultHeap;
		}

		return defaultHeap;
	}

	const HeapFactory::HeapList& HeapFactory::GetHeaps()
	{
		return heaps;
	}

	std::string HeapFactory::WalkTheHeap(const size_t index)
	{
		MEM_FATAL(index < heaps.size(), "Index greater than current size of the HeapFactory");

		const auto& heap = heaps[index];

		const auto name = klib::kString::ToWriter(heap->GetName());
		MEM_INFO("Walking Heap: " + name);

		return heaps[index]->GetStatus();
	}

	std::string HeapFactory::WalkTheDefaultHeap()
	{
		MEM_INFO("Walking Heap: Default");

		return defaultHeap->GetStatus();
	}

	size_t HeapFactory::GetSize()
	{
		return heaps.size();
	}

	void HeapFactory::ReportMemoryLeaks()
	{
		for (HeapBase* heap : heaps)
		{
			if (!heap)
				break;

			if (!heap->GetPrevAddress())
				continue;
			
			ReportMemoryLeaks(heap, 0, heap->GetPrevAddress()->bookmark);
		}
		std::cin.get();
	}

	void HeapFactory::ReportMemoryLeaks(HeapBase* const heap, const size_t minBookmark, const size_t maxBookmark)
	{
		auto* currentHeader = heap->GetPrevAddress();

		if (!currentHeader)
			return;

		if (!AllocHeader::VerifyHeader(currentHeader))
			return;

		while (currentHeader->pPrev && currentHeader->pNext != currentHeader)
		{
			if (currentHeader->bookmark < minBookmark)
			{
				currentHeader = currentHeader->pNext;
				break;
			}

			currentHeader = currentHeader->pPrev;
		}

		if (currentHeader->bookmark > maxBookmark)
			return;

		auto bookmark = currentHeader->bookmark;

		while (kmaths::InRange(bookmark, minBookmark, maxBookmark + 1))
		{
			std::cout << "Heap: \"" << heap->GetName() << "\" id: " << bookmark << " "
				<< "Bytes: " << currentHeader->bytes << "\n";

			currentHeader = currentHeader->pNext;
			
			if (!currentHeader)
				break;

			bookmark = currentHeader->bookmark;
		}
	}

	void HeapFactory::LogTotalBytes(const size_t *bytes) noexcept
	{
		using namespace  kmaths::constants;

		const auto kilo = Divide<float>(*bytes,
			CAST(size_t, kmaths::BytesUnits::KILO));
		const auto mega = Divide<float>(*bytes,
			CAST(size_t, kmaths::BytesUnits::MEGA));
		const auto giga = Divide<float>(*bytes,
			CAST(size_t, kmaths::BytesUnits::GIGA));


		MEM_INFO(klib::kFormat::ToString(
			R"(
Total Heap Bytes:
Gigabytes: {0:3}
Megabytes: {1:3}
Kilobytes: {2:3}
Bytes:     {3:3}
)",
giga,
mega,
kilo,
*bytes))
	}

	void HeapFactory::LogTotalAllocations(const size_t* bytes) noexcept
	{
		using namespace klib::kFormat;

		MEM_INFO(ToString("\nTotal Heap Allocations At Program Shut Down: {0}",
			*bytes));
	}

	HeapFactory::HeapFactory(Token)
	{}
}
