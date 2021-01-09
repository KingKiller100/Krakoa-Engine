#include "Precompile.hpp"
#include "HeapFactory.hpp"

#include "DefaultHeap.hpp"
#include "AllocHeader.hpp"

#include "../MemoryDebug.hpp"
#include "../../Logging/MemoryLogger.hpp"

#include <Maths/BytesUnits.hpp>
#include <Maths/kAlgorithms.hpp>

#include <Utility/FileSystem/kFileSystem.hpp>

#include <iostream>

namespace memory
{
	Heap* HeapFactory::defaultHeap = nullptr;
	HeapFactory::HeapList<HeapFactory::ListSize> HeapFactory::heaps{};

	bool HeapFactory::AddToParent(const char* parentName, Heap* pChild)
	{
		auto* parentHeap = FindHeap(parentName);

		if (!parentName)
			return false;

		auto& firstChild = parentHeap->GetFamily().pFirstChild;

		pChild->GetFamily().pParent = parentHeap;

		if (!firstChild)
			firstChild = pChild;
		else
		{
			firstChild->GetFamily().pPrevSibling = pChild;

			auto& fam = pChild->GetFamily();
			fam.pNextSibling = firstChild;

			parentHeap->GetFamily().pFirstChild = pChild;
		}

		return true;
	}

	void HeapFactory::Initialize() noexcept
	{
		using namespace klib;

		std::filesystem::path path(kFileSystem::GetExeDirectory());
		path /= "Logs\\Memory.log";

		if (kFileSystem::CheckFileExists(path.string()))
		{
			kFileSystem::Remove(path);
		}

		MEM_INIT_LOGS(R"(Definitions:
   Object - "Structured piece of data in memory"
   Block -  "Allocated space of an object in memory 
   including AllocHeader & end marker"
)");

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
			MEM_INF(heap->GetStatus());

			heap->DeleteLeaks();
			free(heap);
			heap = nullptr;
		}

		totalBytes += defaultHeap->GetTotalAllocatedBytes();
		totalAllocations += defaultHeap->WalkTheHeap();
		const auto status = defaultHeap->GetStatus();
		MEM_INF(status);

		LogTotalBytes(totalBytes);
		LogTotalAllocations(totalAllocations, defaultHeap->GetLastBookmark());
	}

	Heap* HeapFactory::GetDefaultHeap() noexcept
	{
		static Heap_VFTBL localVFTBL(GetDefaultHeapStatus);

		if (!defaultHeap)
		{
			defaultHeap = static_cast<Heap*>(malloc(sizeof(Heap)));
			defaultHeap->Initialize("Default", &localVFTBL);
			heaps.front() = defaultHeap;
		}

		return defaultHeap;
	}

	const HeapFactory::HeapList<HeapFactory::ListSize>& HeapFactory::GetHeaps()
	{
		return heaps;
	}

	std::string HeapFactory::WalkTheHeap(const size_t index)
	{
		MEM_ASSERT(index < heaps.size(), "Index greater than current size of the HeapFactory");

		const auto& heap = heaps[index];

		const auto name = klib::kString::ToWriter(heap->GetName());
		MEM_INF("Walking Heap: " + name);

		return heaps[index]->GetStatus();
	}

	std::string HeapFactory::WalkTheDefaultHeap()
	{
		MEM_INF("Walking Heap: Default");

		return defaultHeap->GetStatus();
	}

	size_t HeapFactory::GetSize()
	{
		return heaps.size();
	}

	void HeapFactory::ReportMemoryLeaks()
	{
		for (Heap* heap : heaps)
		{
			if (!heap)
				break;

			if (!heap->GetPrevAddress())
				continue;

			ReportMemoryLeaks(heap, 0, heap->GetPrevAddress()->bookmark);
		}
		std::cin.get();
	}

	void HeapFactory::ReportMemoryLeaks(Heap* const heap, const size_t minBookmark, const size_t maxBookmark)
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

	Heap* HeapFactory::FindHeap(const char* name)
	{
		for (auto& heap : heaps)
		{
			if (!heap)
				return nullptr;

			std::string_view currentHeapName = heap->GetName();

			if (currentHeapName.compare(name))
				return heap;
		}

		return nullptr;
	}

	void HeapFactory::LogTotalBytes(const size_t bytes) noexcept
	{
		using namespace  kmaths::constants;

		const auto totalDeallocations = Heap::s_TotalLifetimeBytesAllocated - bytes;
		const auto kilo = Divide<float>(bytes,
			CAST(size_t, kmaths::BytesUnits::KILO));
		const auto mega = Divide<float>(bytes,
			CAST(size_t, kmaths::BytesUnits::MEGA));
		const auto giga = Divide<float>(bytes,
			CAST(size_t, kmaths::BytesUnits::GIGA));


		MEM_INF(klib::ToString(
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
bytes));

		MEM_INF(klib::ToString("LifeTime Heap Bytes Allocations: {0}"
			, Heap::s_TotalLifetimeBytesAllocated));

		MEM_INF(klib::ToString("Lifetime Heap Bytes Deallocations: {0}"
			, totalDeallocations));
	}

	void HeapFactory::LogTotalAllocations(const size_t active, const size_t total) noexcept
	{
		using namespace klib::kString;

		MEM_INF(ToString("Total Active Heap Allocations: {0}"
			, active));

		MEM_INF(ToString("Total Lifetime Heap Allocations: {0}"
			, total));
	}

	HeapFactory::HeapFactory(Token)
	{}
}
