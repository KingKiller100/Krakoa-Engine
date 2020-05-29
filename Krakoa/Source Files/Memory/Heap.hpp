﻿#pragma once

#include <HelperMacros.hpp>

#include <string>

namespace memory
{
	class Heap final
	{
	public:
		explicit Heap(const char* name) noexcept;
		~Heap() noexcept;

		void Initialize(const char* n) noexcept;

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

		USE_RESULT std::string GetStatus() const;
		USE_RESULT size_t WalkTheHeap() const noexcept;

		void SetPrevAddress(void* prev) noexcept;
		USE_RESULT void* GetPrevAddress() const noexcept;

		
	private:
		const char* name;
		size_t totalBytes;
		void * pPrevAddress;
	};
}
