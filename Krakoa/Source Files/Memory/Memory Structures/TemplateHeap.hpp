#pragma once

#include "HeapBase.hpp"

#include "MemoryTypes.hpp"

#include <Utility/Format/kFormatToString.hpp>

#include <string>

namespace memory
{
	template<typename T>
	class TemplateHeap final : public HeapBase
	{
	public:
		using Type = T;
		static constexpr auto TypeBytes = sizeof(Type);
		
		explicit TemplateHeap() noexcept
			: HeapBase(typeid(Type).name())
		{}

		~TemplateHeap() noexcept
			= default;

		USE_RESULT constexpr size_t GetBytesPerObject() const
		{
			return TypeBytes;
		}
	};

	template<typename T>
	using THeap = TemplateHeap<T>; // Template Heap

	template<typename T>
	static void CallObjectDestructor(void* pMemPtr)
	{
		auto& ref = *REINTERPRET(T*, pMemPtr);

		ref.~T();
	}

	template<typename T>
	static std::string GetTemplateHeapStatus(const HeapBase* pHeap)
	{
		using namespace klib::kFormat;
		using Heap = TemplateHeap<T>;
		
		constexpr size_t bytesPerObj = Heap::TypeBytes;

		const auto name = pHeap->GetName();
		const auto totalBytes = pHeap->GetTotalAllocatedBytes();

		const size_t count = pHeap->WalkTheHeap();
		const size_t multiplier = (count ? count : 1);

		const size_t bytesPerBlock = (pHeap->GetTotalAllocatedBytes() / multiplier);
		const size_t totalBytesOfThisObject = bytesPerObj * multiplier;

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
	
	/*template<typename T>
	Heap_VTBL templateHeapVTBL = Heap_VTBL(
		GetTemplateHeapStatus<T>,
		CallObjectDestructor<T>
	);*/
}
