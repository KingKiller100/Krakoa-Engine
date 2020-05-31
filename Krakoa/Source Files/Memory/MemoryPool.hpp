﻿#pragma once

#include "Memory Structures/MemoryTypes.hpp"

#include <HelperMacros.hpp>
#include <Maths/BytesTypes.hpp>

#include <array>
#include <string>

namespace memory
{
	struct SubPool
	{
		void* pHead;
		Byte_Ptr_Type pNextFree;
		size_t capacity;

		SubPool() noexcept
			: pHead(nullptr),
			pNextFree(nullptr),
			capacity(0)
		{}
		
		SubPool(const SubPool&) = delete;
	};

	class MemoryPool
	{
		struct Token {};
		static constexpr size_t PoolSize = 4;
		using SubPoolList = std::array<SubPool, PoolSize>;

	public:
		MemoryPool(Token&) noexcept;
		MemoryPool(const MemoryPool&) = delete;
		~MemoryPool() noexcept;

		void Initialize(const size_t volume, const kmaths::BytesUnits units);

		Byte_Ptr_Type Allocate(const size_t bytes);
		void Deallocate(AllocHeader* pHeader, const size_t bytes);

		USE_RESULT size_t GetTotalBytes() const;
		USE_RESULT size_t GetMaxBytes() const;

		USE_RESULT size_t WalkTheHeap() const;
		USE_RESULT std::string GetStatus() const;

		static MemoryPool& Reference();

	private:
		/**
		 * \brief
		 *		Inform's current pool if there is enough space inside
		 * \param requestedBytes
		 *		Bytes to allocate
		 * \return
		 *		TRUE if this pool has space for this object or FALSE if this pool has no more space
		 */
		USE_RESULT int GetSubPoolIndex(const size_t requestedBytes) const;

		void CreateNewPool(const size_t capacity, const size_t index);
		void ShutDown();
		
	private:
		SubPoolList subPools;
		size_t currentIndex;
		bool active = false;
	};
}

