#include "Precompile.hpp"
#include "HeapFactory.hpp"

#include <Maths/BytesTypes.hpp>


#include "DefaultHeap.hpp"

#include "../../Core/Logging/MemoryLogger.hpp"

#include <Utility/File System/kFileSystem.hpp>
#include <Utility/String/kStringManipulation.hpp>
#include <Maths/Constants.hpp>


namespace memory
{
	HeapBase* HeapFactory::defaultHeap = nullptr;
	HeapFactory::HeapList HeapFactory::heaps;

	void HeapFactory::Initialize() noexcept
	{
		using namespace klib;

		const auto currentDir = kFileSystem::GetExeDirectory();

		if (kFileSystem::CheckFileExists(currentDir + "Logs\\Memory.log"))
		{
			kFileSystem::RemoveFile(currentDir + "Logs\\Memory.log");
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

		for (auto& heap : heaps)
		{
			totalBytes += heap->GetTotalAllocatedBytes();
			totalAllocations += heap->WalkTheHeap();
			MEM_INFO(heap->GetStatus());
			free(heap);
		}

		MEM_INFO(defaultHeap->GetStatus());
		totalBytes += defaultHeap->GetTotalAllocatedBytes();
		totalAllocations += defaultHeap->WalkTheHeap();

		heaps.clear();

		LogTotalBytes(&totalBytes);
		LogTotalAllocations(&totalAllocations);
	}

	HeapBase* HeapFactory::GetDefaultHeap() noexcept
	{
		static Heap_VFTBL localVFTBL(GetDefaultHeapStatus);

		if (!defaultHeap)
		{
			defaultHeap = static_cast<HeapBase*>(malloc(sizeof(DefaultHeap)));
			defaultHeap->Initialize("Default", &localVFTBL);
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

	void HeapFactory::LogTotalBytes(const size_t *bytes) noexcept
	{
		using namespace  kmaths::constants;

		const auto kilo = Divide<float>(*bytes,
			CAST(size_t, kmaths::BytesType::KILO));

		const auto mega = Divide<float>(*bytes,
			CAST(size_t, kmaths::BytesType::MEGA));

		const auto giga = Divide<float>(*bytes,
			CAST(size_t, kmaths::BytesType::GIGA));


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
