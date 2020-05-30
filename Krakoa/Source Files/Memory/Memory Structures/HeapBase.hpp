#pragma once

#include <HelperMacros.hpp>

#include <string>
#include <functional>

namespace memory
{
	class HeapBase;
	
	struct Heap_VTBL
	{
		std::function<std::string(const HeapBase*)> getStatusFunc;
		std::function<void(void*)> callObjFunc;

		Heap_VTBL(std::function<std::string(const HeapBase*)> statusFunc, std::function<void(void*)> callObjFunc)
			: getStatusFunc(std::move(statusFunc)),
		callObjFunc(std::move(callObjFunc))
		{}
	};
	
	class HeapBase 
	{
	public:
		explicit HeapBase(const char* name) noexcept;
		~HeapBase() noexcept = default;

		void Initialize(const char* n, Heap_VTBL * heapVTBL) noexcept;

		template<size_t N>
		void SetName(const char(&n)[N]) noexcept
		{
			name = n;
		}
		void SetName(const char* n) noexcept;
		USE_RESULT const char* GetName() const noexcept;


		void Allocate(const size_t bytes) noexcept;
		void Deallocate(const size_t bytes) noexcept;
		USE_RESULT size_t GetTotalAllocatedBytes() const noexcept;

		USE_RESULT size_t WalkTheHeap() const noexcept;

		void SetPrevAddress(void* prev) noexcept;
		USE_RESULT void* GetPrevAddress() const noexcept;

		USE_RESULT std::string GetStatus() const noexcept;
		void CallObjectDestructor(void* pMemPtr) const noexcept;
		
	protected:
		const char* name;
		size_t totalBytes;
		void * pPrevAddress;
		Heap_VTBL* vtbl;
	};
}
