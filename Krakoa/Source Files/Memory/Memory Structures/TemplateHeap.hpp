#pragma once

#include "Heap.hpp"

#include <Utility/String/kToString.hpp>

#include <string>

namespace memory
{
	template<typename T>
	class TemplateHeap final : public Heap
	{
	public:
		using Type = T;
		static constexpr auto TypeSize = sizeof(Type);

		~TemplateHeap() noexcept
			= default;

		USE_RESULT constexpr size_t GetBytesPerObject() const
		{
			return TypeSize;
		}
	};

	template<typename T>
	static std::string GetTemplateHeapStatus(const Heap* pHeap)
	{
		using namespace klib::kString;

		const auto name = pHeap->GetName();
		const size_t count = pHeap->WalkTheHeap();

		if (!count)
			return ToString("Heap \"{0}\" is empty\n", name);

		constexpr size_t bytes_per_object = TemplateHeap<T>::TypeSize;
		const size_t bytes_total = bytes_per_object * count;

		const auto totalBytes = pHeap->GetTotalAllocatedBytes();
		const size_t bytes_per_block = (totalBytes / count);

		std::string report = ToString(R"(Heap "{0}"
Count:              {1}
Bytes Per Object:   {2}
Bytes Per Block:    {4}
Total Object:       {3}
Total Block:        {5}
)",
name
, count
, bytes_per_object
, bytes_total
, bytes_per_block
, totalBytes);

		return report;
	}

	template<typename T>
	Heap_VFTBL templateHeapVFTBL = Heap_VFTBL(
		GetTemplateHeapStatus<T>
	);
}
