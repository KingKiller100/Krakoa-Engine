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
	HeapFactory::HeapList HeapFactory::heaps{};
	std::atomic_uint16_t HeapFactory::activeHeapIndex{ 1 };

	// bool HeapFactory::AddToParent(const char* parentName, Heap* pChild)
	// {
	// 	auto* parentHeap = FindHeap(parentName);
	//
	// 	if (!parentName)
	// 		return false;
	//
	// 	auto& firstChild = parentHeap->GetFamily().pFirstChild;
	//
	// 	pChild->GetFamily().pParent = parentHeap;
	//
	// 	if (!firstChild)
	// 		firstChild = pChild;
	// 	else
	// 	{
	// 		firstChild->GetFamily().pPrevSibling = pChild;
	//
	// 		auto& fam = pChild->GetFamily();
	// 		fam.pNextSibling = firstChild;
	//
	// 		parentHeap->GetFamily().pFirstChild = pChild;
	// 	}
	//
	// 	return true;
	// }

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
			heap->ShutDown();
			free(heap);
			heap = nullptr;
		}

		totalBytes += defaultHeap->GetTotalAllocatedBytes();
		totalAllocations += defaultHeap->WalkTheHeap();
		const auto status = defaultHeap->GetStatus();
		MEM_INF(status);

		LogTotalBytes(totalBytes);
		LogTotalAllocations(totalAllocations, GetTotalAllocationsCount());
	}

	Heap* HeapFactory::GetDefaultHeap() noexcept
	{
		static Heap_VFTBL localVFTBL(GetDefaultHeapStatus);

		if (!defaultHeap)
		{
			defaultHeap = memory::Allocate<Heap>();
			defaultHeap->Initialize("Default", &localVFTBL);
			heaps[0] = defaultHeap;
		}

		return defaultHeap;
	}

	const HeapFactory::HeapList& HeapFactory::GetHeaps()
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

			ReportMemoryLeaks(heap);
		}
	}

	Heap* HeapFactory::CreateHeapImpl(const char* name, size_t bytes, Heap_VFTBL* vtbl)
	{
		auto* heap = Allocate<Heap>(bytes);
		heap->Initialize(name, vtbl);
		heaps[activeHeapIndex++] = heap;
		return heap;
	}

	void HeapFactory::ReportMemoryLeaks(Heap* const heap)
	{
		auto* allocList = heap->GetAllocList<patterns::BiDirectionalLinkedList<AllocHeader>>();

		if (!allocList)
			return;

		if (!allocList->head)
			return;

		size_t totalBytes = 0;
		const auto heapName = heap->GetName();

		std::stringstream stream;

		auto AppendLeakToStreamFunc = [&stream](const AllocHeader& header)
		{
			static size_t count(0);
			stream << "Heap: \"" << header.pHeap->GetName() << "\" id: " << header.bookmark << " ";
			stream << "Bytes: " << header.bytes << "\n";
			if (++count > 500)
			{
				std::cout << stream.view();
				stream.clear();
				count = 0;
			}
		};

		auto currentNode = allocList->head;
		size_t count = 0;
		while (currentNode != allocList->tail)
		{
			if (!currentNode)
			{
				std::cout << heapName << " chain is broken: " << 
					count << "/" << heap->GetCount();
				break;
			}
			
			const auto& header = currentNode->data;
			AppendLeakToStreamFunc(header);
			totalBytes += header.bytes;
			currentNode = currentNode->next;
			++count;
		}

		totalBytes += allocList->tail->data.bytes;
		AppendLeakToStreamFunc(allocList->tail->data);

		std::cout << stream.view();

		std::cout << "Heap: \"" << heapName << "\" | "
			<< "Total Bytes: " << totalBytes << 
			" | Total Alive Count: " << heap->GetCount() << "\n";
	}

	Heap* HeapFactory::FindHeap(const char* name)
	{
		for (auto& heap : heaps)
		{
			if (!heap)
				return nullptr;

			const std::string_view currentHeapName = heap->GetName();

			if (currentHeapName == name)
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

		MEM_INF(klib::ToString("Total allocations: {0}"
			, GetTotalAllocationsCount()));
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
