#pragma once

#include "Heap.hpp"

#include <Utility/Format/kFormatToString.hpp>

#include <string>

namespace memory
{
	template<typename T>
	class TemplateHeap final : public Heap
	{
	public:
		using Type = T;
		static constexpr auto TypeSize = sizeof(Type);

		explicit TemplateHeap() noexcept
			: Heap(typeid(Type).name())
		{}

		~TemplateHeap() noexcept
			= default;

		USE_RESULT constexpr size_t GetBytesPerObject() const
		{
			return TypeBytes;
		}
	};

	template<typename T>
	static std::string GetTemplateHeapStatus(const Heap* pHeap)
	{
		using namespace klib::kFormat;
		using Heap = TemplateHeap<T>;

		constexpr size_t bytesPerObj = Heap::TypeSize;

		const auto name = pHeap->GetName();
		const auto totalBytes = pHeap->GetTotalAllocatedBytes();

		const size_t count = pHeap->WalkTheHeap();

		if (!count)
			return ToString("Heap \"{0}\" is empty\n", name);

		const size_t bytesPerBlock = (pHeap->GetTotalAllocatedBytes() / count);
		const size_t totalBytesOfThisObject = bytesPerObj * count;

		std::string report;

		report.append(ToString(R"(Heap "{0}"
Object Bytes: {2}
Total Object Bytes: {4}
Block Bytes: {3}
Total Block Bytes: {5}
Total Number of Allocations: {1}
)",
name,
count,
bytesPerObj,
bytesPerBlock,
totalBytesOfThisObject,
totalBytes));


		return report;
	}

	template<typename T>
	Heap_VFTBL templateHeapVFTBL = Heap_VFTBL(
		GetTemplateHeapStatus<T>
	);
}
