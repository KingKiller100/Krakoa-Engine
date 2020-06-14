#pragma once

#include <HelperMacros.hpp>

#include <string>

namespace memory
{
	struct Heap_VFTBL;
	struct AllocHeader;
	
	class HeapBase 
	{
	public:
		explicit HeapBase(const char* name) noexcept;
		HeapBase(const HeapBase&) = delete;
		
		~HeapBase() noexcept = default;

		void Initialize(const char* n, Heap_VFTBL * heapVTBL) noexcept;

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

		USE_RESULT size_t WalkTheHeap() const;
		void DeleteLeaks();

		void SetPrevAddress(AllocHeader* prev) noexcept;
		USE_RESULT AllocHeader* GetPrevAddress() const noexcept;

		USE_RESULT std::string GetStatus() const;
		
	protected:
		const char* name;
		size_t totalBytes;
		AllocHeader* pPrevAddress;
		Heap_VFTBL* vftbl;
	};

	// Virtual Function Table for Heaps
	struct Heap_VFTBL
	{
	public:
		typedef std::string(*GetStatusFunc)(const HeapBase*);

		explicit Heap_VFTBL(GetStatusFunc statusFunc)
			: getStatusFunc(std::move(statusFunc))
		{}

	public:
		GetStatusFunc getStatusFunc = nullptr;
	};
}
