#pragma once


#include "../PointerTypes.hpp"
#include "../Core/Logging/CoreLogger.hpp"

#include <HelperMacros.hpp>
#include <Utility/String/kToString.hpp>

#include <atomic>
#include <thread>
#include <type_traits>

namespace patterns
{
	template<typename T, size_t Max>
	class ObjectPool
	{
	public:
		using Type = T;

		static constexpr auto MaxLength = Max;
		static constexpr auto TypeSize = sizeof(T);
		static constexpr auto Bytes = TypeSize * MaxLength;

		ObjectPool()
			: size(0)
		{
			for (auto i = 0; i < MaxLength; ++i)
				pool[i].next = &pool[i+1];

			nextFree = &pool[0];
		}

		ObjectPool(ObjectPool&& other) noexcept
		{
			*this = std::move(other);
		}

		ObjectPool& operator=(ObjectPool&& other) noexcept
		{
			if (this == &other)
				return *this;

			pool = std::move(other.pool);
			nextFree.store(std::move(other.nextFree));

			other.nextFree = nullptr;

			return *this;
		}

		template<typename ...Args, typename = std::enable_if_t<std::is_constructible_v<Type, Args...>>>
		USE_RESULT Type* Create(Args&& ...params)
		{
			return new (Allocate()) Type(std::forward<Args>(params)...);
		}

		void Destroy(Type* ptr)
		{
			if (!ptr)
				return;

			ptr->~T();

			Deallocate(ptr);
		}

		USE_RESULT size_t GetSize() const noexcept
		{
			return size;
		}

		USE_RESULT constexpr size_t GetMaxSize() const noexcept
		{
			return MaxLength;
		}


		ObjectPool(const ObjectPool& other) = delete;
		ObjectPool& operator =(const ObjectPool& other) = delete;

	private:
		USE_RESULT Type* Allocate()
		{
			const auto threadID = GetThreadID();

			auto* node = nextFree.load();
			while (node && !nextFree.compare_exchange_weak(node, node->next, std::memory_order_seq_cst))
			{
				const auto msg = klib::kString::ToString("Thread {0} failed to allocate", threadID);
				KRK_DBUG(msg);
			}

			KRK_DBUG(klib::kString::ToString("Thread {0} successfully allocatedd", threadID));

			if (nextFree == nullptr)
				throw std::bad_alloc();

			size++;

			return REINTERPRET(Type*, &node->storage);
		}

		void Deallocate(Type* ptr)
		{
			const auto threadID = GetThreadID();
			
			const auto node = REINTERPRET(Node*, ptr);

			node->next = nextFree;
			while (!nextFree.compare_exchange_weak(node->next, node, std::memory_order_seq_cst))
			{
				const auto msg = klib::kString::ToString("Thread {0} failed to deallocate", threadID);
				KRK_DBUG(msg);
			}

			KRK_DBUG(klib::kString::ToString("Thread {0} successfully deallocatedd", threadID));

			size--;
		}

		USE_RESULT constexpr _Thrd_id_t GetThreadID(std::thread::id threadID = std::this_thread::get_id()) const noexcept
		{
			return *REINTERPRET(_Thrd_id_t*, &threadID);
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
