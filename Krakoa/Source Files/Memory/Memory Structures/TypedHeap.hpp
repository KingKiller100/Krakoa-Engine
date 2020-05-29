#pragma once

#include "HeapBase.hpp"

#include "MemoryTypes.hpp"

#include <Utility/Format/kFormatToString.hpp>

#include <string>

namespace memory
{
	template<typename T>
	class TypedHeap final : public HeapBase
	{
	public:
		using Type = T;
		static constexpr auto TypeBytes = sizeof(Type);
		
		explicit TypedHeap() noexcept
			: HeapBase(typeid(Type).name())
		{}

		~TypedHeap() noexcept
			= default;

		USE_RESULT constexpr size_t GetBytesPerObject() const
		{
			return TypeBytes;
		}
	};

	template<typename T>
	static void CallObjectDestructor(void* pMemPtr)
	{
		auto ptr = REINTERPRET(T*, pMemPtr);

		ptr->~T();
	}

	template<typename T>
	static std::string GetTypedHeapStatus(HeapBase* pHeap)
	{
		using namespace klib::kFormat;

		const auto name = pHeap->GetName();
		const auto totalBytes = pHeap->GetTotalAllocatedBytes();

		auto typeHeap = REINTERPRET(TypedHeap<T>*, pHeap);
		const auto bytesPerObj = typeHeap->GetBytesPerObject();

		
		const auto count = pHeap->WalkTheHeap();
		const auto multiplier = (count ? count : 1);

		const auto bytesPerBlock = (pHeap->GetTotalAllocatedBytes() / multiplier);
		const auto totalBytesOfThisObject = bytesPerObj * multiplier;

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
bytesPerObj,
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
bytesPerObj,
totalBytes));
		}
		else
			details.append(ToString("Heap \"{0}\" is empty\n", name));


		return details;
	}

	template<typename T>
	const Heap_VTBL typeHeapVTBL = { GetTypedHeapStatus<T>, CallObjectDestructor<T> };
}
