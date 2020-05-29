#pragma once

#include <HelperMacros.hpp>

#include <string>

namespace memory
{
	struct Heap_VTBL
	{
		typedef std::string(*GetStatusFunc)();
		typedef void(*CallObjectDestructorFunc)(void*);

		GetStatusFunc getStatusFunc;
		CallObjectDestructorFunc callObjFunc;
	};
	
	class HeapBase 
	{
	public:
		explicit HeapBase(const char* name) noexcept;
		~HeapBase() noexcept;

		void Initialize(const char* n, Heap_VTBL *heapVTBL) noexcept;

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
