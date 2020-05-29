#pragma once

#include <HelperMacros.hpp>

#include "../PointerTypes.hpp"

#include <atomic>
#include <thread>

namespace patterns
{
	template<typename T, size_t Max = 1000>
	class ObjectPool
	{
	public:
		using Type = T;
		using Pointer = Type * ;

		static constexpr auto Length = Max;
		static constexpr auto TypeSize = sizeof(T);
		static constexpr auto Bytes = TypeSize * Length;

		ObjectPool()
			: size(0)
		{
			for (auto i = 0; i < Max; ++i)
				pool[i - 1].next = &pool[i];

			nextFree = &pool[0];
		}

		ObjectPool(ObjectPool&& other) noexcept
			: pool(std::move(other.pool)),
			nextFree(std::move(other.nextFree)),
			size(other.size)
		{
			other.nextFree = nullptr;
		}

		ObjectPool& operator=(ObjectPool&& other) noexcept
		{
			if (this == &other)
				return *this;

			pool = std::move(other.pool);
			nextFree = std::move(other.nextFree);

			other.nextFree = nullptr;

			return *this;
		}


		template<typename ...Args>
		USE_RESULT Pointer Create(Args&& ...params)
		{
			return new (Allocate()) Type(std::forward<Args>(params)...);
		}

		void Destroy(Pointer ptr)
		{
			if (!ptr)
				return;

			ptr->~Type();

			Deallocate(ptr);
		}

		USE_RESULT size_t GetSize() const noexcept
		{
			return size;
		}

		ObjectPool(const ObjectPool& other) = delete;
		ObjectPool& operator =(const ObjectPool& other) = delete;

	private:
		USE_RESULT Pointer Allocate()
		{
			auto thisThreadID = std::this_thread::get_id();
			const auto idPtr = REINTERPRET(_Thrd_id_t*, &thisThreadID);

			const auto node = nextFree.load();
			while (node && !nextFree.compare_exchange_weak(node, node->next, std::memory_order_seq_cst))
			{
				const auto msg = klib::kFormat::ToString("Thread {0} failed to allocate", *idPtr);
				KRK_DBUG(msg);
			}

			KRK_DBUG(klib::kFormat::ToString("Thread {0} successfully allocatedd", *idPtr));

			if (nextFree == nullptr)
				throw std::bad_alloc();

			size++;

			return REINTERPRET(Pointer, &node->storage);
		}

		void Deallocate(Pointer ptr)
		{
			if (!ptr)
				return;

			const auto node = REINTERPRET(Node*, ptr);

			auto thisThreadID = std::this_thread::get_id();
			const auto idPtr = REINTERPRET(_Thrd_id_t*, &thisThreadID);

			node->next = nextFree;
			while (!nextFree.compare_exchange_weak(node->next, node, std::memory_order_seq_cst))
			{
				const auto msg = klib::kFormat::ToString("Thread {0} failed to deallocate", *idPtr);
				KRK_DBUG(msg);
			}

			KRK_DBUG(klib::kFormat::ToString("Thread {0} successfully deallocatedd", *idPtr));

			size--;
		}

	private:
		union Node
		{
			std::aligned_storage_t<TypeSize, alignof(Type)> storage;
			Node* next = nullptr;
		};

		krakoa::Solo_Ptr<Node[]> pool = krakoa::Make_Solo<Node[]>(Max);
		std::atomic<Node*> nextFree = nullptr;
		size_t size;
	};
}
