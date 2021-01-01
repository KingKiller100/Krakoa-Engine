#pragma once

#include <HelperMacros.hpp>
#include <Maths/BytesUnits.hpp>

#include <array>
#include <string>

#include "MemoryStructures/MemoryLinkedList.hpp"

namespace memory
{
	class HeaderList : public MemoryLinkedList<HeaderList>
	{};
	
	
	struct SubPool
	{
	public:
		explicit SubPool(const size_t capacity = 0) noexcept
			: pStartAddress(nullptr),
			ppHead(nullptr),
			pNextFree(nullptr),
			capacity(capacity)
			, remainingSpace(capacity)
		{}

		SubPool(SubPool&& other) noexcept
			: pStartAddress(other.pStartAddress),
			pNextFree(other.pNextFree),
			ppHead(other.ppHead),
			capacity(other.capacity),
			remainingSpace(other.remainingSpace)
		{ }

		SubPool& operator=(SubPool&& other) noexcept
		{
			pStartAddress = other.pStartAddress;
			pNextFree = other.pNextFree;
			ppHead = other.ppHead;
			remainingSpace = other.remainingSpace;

			auto* capPtr = const_cast<size_t*>(&capacity);
			*capPtr = other.capacity;

			return *this;
		}

		SubPool(const SubPool&) = delete;

	public:
		void* pStartAddress;
		HeaderList** ppHead;
		kmaths::Byte_Type* pNextFree;
		const size_t capacity;
		size_t remainingSpace;
	};

	class MemoryPool
	{
		static constexpr size_t SubPoolSize = 4;
		
	public:
		MemoryPool(const size_t initialVolume, const size_t typeSize);
		~MemoryPool() noexcept;

		kmaths::Byte_Type* Allocate(const size_t requestedBytes);
		void Deallocate(void* ptr, const size_t objectBytesToDelete);

		USE_RESULT size_t GetBytes() const;
		USE_RESULT size_t GetMaxBytes() const;
		USE_RESULT bool IsEmpty() const noexcept;
		USE_RESULT size_t GetAllocationCount() const noexcept;

		USE_RESULT std::string GetStatus(const char* type) const;

		MemoryPool(const MemoryPool&) = delete;
		MemoryPool& operator=(const MemoryPool&) noexcept = delete;
	private:
		void ShutDown();

		kmaths::Byte_Type* FindFreeBlock(SubPool& pool, const size_t requestedBytes) const;
		void AddNewLink(SubPool& pool, void* pNextBlock, size_t bytes) const;
		bool CheckBlockIsDead(const kmaths::Byte_Type* pNextFree, size_t requestedBytes) const;

		/**
		 * \brief
		 *		Inform's current pool if there is enough space inside
		 * \param requestedBytes
		 *		Bytes to allocate
		 * \return
		 *		TRUE if this pool has space for this object or FALSE if this pool has no more space
		 */
		USE_RESULT kmaths::Byte_Type* GetBlockStartPtr(const size_t requestedBytes);
		
		void CreateNewPool(const size_t capacity, const size_t index);
		static void MoveNextFreePointer(kmaths::Byte_Type*& pNextFree);

		SubPool& FindOwner(void* pBlock);

	private:
		std::array<SubPool, SubPoolSize> subPoolList;
	};
}

