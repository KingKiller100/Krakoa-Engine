#pragma once

#include <HelperMacros.hpp>
#include "../../Patterns/LinkedList.hpp"

#include <string>

namespace memory
{

	struct Heap_VFTBL;
	struct AllocHeader;

	class Heap
	{
	protected:
		struct Family {
			Heap* pParent;
			Heap* pFirstChild;
			Heap* pPrevSibling;
			Heap* pNextSibling;
		};

	public:
		Heap(Heap&&) noexcept = default;
		Heap& operator=(Heap&&) noexcept = default;

		~Heap() noexcept = default;

		void Initialize(const char* n, Heap_VFTBL* heapVTBL) noexcept;
		void ShutDown();

		template<size_t N>
		void SetName(const char(&n)[N]) noexcept
		{
			name = n;
		}
		void SetName(const char* n) noexcept;
		USE_RESULT const char* GetName() const noexcept;

		// bool AddToParent(Heap* pParent);
		// USE_RESULT const Family& GetFamily() const noexcept;
		// USE_RESULT Family& GetFamily() noexcept;

		void IncrementAllocationsCount();
		void DecrementAllocationsCount();
		USE_RESULT size_t GetCount() const;
		
		void AllocateBytes(const size_t bytes) noexcept;
		void DeallocateBytes(const size_t bytes) noexcept;
		USE_RESULT size_t GetTotalAllocatedBytes() const noexcept;

		USE_RESULT size_t WalkTheHeap() const;
		USE_RESULT size_t GetLastBookmark() const;

		void DeleteLeaks() const;

		template<typename T>
		USE_RESULT T* GetAllocList() const noexcept
		{
			return static_cast<T*>(allocListVoid);
		}

		USE_RESULT std::string GetStatus() const;

		Heap(const Heap&) = delete;
		Heap& operator=(const Heap&) = delete;

	public:
		static size_t s_TotalLifetimeBytesAllocated;
	protected:
		const char* name;
		size_t totalBytes;
		size_t allocationsCount;
		void* allocListVoid;
		Heap_VFTBL* vftbl;

		// Family family;

	};

	// Virtual Function Table for Heaps
	struct Heap_VFTBL
	{
	public:
		typedef std::string(*GetStatusFunc)(const Heap*);

		explicit Heap_VFTBL(GetStatusFunc statusFunc)
			: getStatusFunc(std::move(statusFunc))
		{}

	public:
		GetStatusFunc getStatusFunc = nullptr;
	};
}
