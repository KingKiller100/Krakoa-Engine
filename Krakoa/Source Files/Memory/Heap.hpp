#pragma once

#include <HelperMacros.hpp>

#define MEMSYSTEM_SIGNATURE 0xdeadc0de // 3735929054
#define MEMSYSTEM_ENDMARKER 0xdeadbeef // 3735928559

namespace krakoa
{
	class Heap final
	{
	public:
		explicit Heap(const char* name) noexcept;
		~Heap() noexcept;

		template<size_t N>
		void SetName(const char (& n)[N])
		{
			name = n;
		}
		void SetName(const char* n);
		USE_RESULT const char* GetName() const;


		void Allocate(const size_t bytes);
		void Deallocate(const size_t bytes);
		USE_RESULT size_t GetTotalAllocatedBytes() const;

		void WalkHeap();
		
		USE_RESULT void* GetPrevAddress() const;

		static Heap* Create(const char* name);
		
	private:
		const char* name;
		size_t totalBytes;
		void * prevAddress;
	};

	struct AllocHeader
	{
		size_t signature;
		Heap* pHeap;
		size_t size;
		AllocHeader* pPrev;
		AllocHeader* pNext;
	};
}
