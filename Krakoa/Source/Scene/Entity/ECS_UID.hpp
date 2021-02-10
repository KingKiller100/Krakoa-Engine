#pragma once

#include <Template/kSimpleOperators.hpp>

#include <string>
#include <cstdint>
#include <limits>

#ifdef max
#	undef max
#endif

namespace krakoa::scene::ecs
{
	class EntityUID : klib::kTemplate::SimpleComparisonOperators<EntityUID>
	{
	public:
		using ID_t = std::uint64_t;

	private:
		static constexpr auto s_Null = std::numeric_limits<ID_t>::max();

	public:
		constexpr EntityUID(ID_t val = s_Null) noexcept
			: id(val)
		{}

		constexpr EntityUID(const EntityUID&) = default;
		constexpr EntityUID& operator=(const EntityUID&) = default;

		constexpr EntityUID(EntityUID&& other) noexcept
		{
			*this = std::move(other);
		}

		constexpr EntityUID& operator=(EntityUID&& other) noexcept
		{
			if (std::addressof(other) != this)
			{
				id = std::move(other.id);
				other.id = s_Null;
			}

			return *this;
		}

		~EntityUID() noexcept = default;
		
		void Nullify() noexcept;

		bool IsNull() const noexcept;

		ID_t GetValue() const;

		operator bool() const noexcept
		{
			return !IsNull();
		}
		
		template<typename T>
		constexpr operator T() const noexcept
		{
			return static_cast<T>(GetValue());
		}

		template<typename T>
		constexpr operator T* () const noexcept
		{
			return (T*)GetValue();
		}

		EntityUID Mask() const;

		[[nodiscard]] std::string ToString() const noexcept;

		ID_t operator++(int);

		ID_t operator++();

		ID_t operator--(int);

		ID_t operator--();

		constexpr bool operator==(const EntityUID& other) const noexcept
		{
			return other.id == id;
		}

		constexpr bool operator!=(const EntityUID& other) const noexcept
		{
			return other.id != id;
		}

		constexpr bool operator<(const EntityUID& other) const noexcept
		{
			return id < other.id;
		}

		constexpr bool operator<=(const EntityUID& other) const noexcept
		{
			return id < other.id;
		}

		constexpr bool operator>(const EntityUID& other) const noexcept
		{
			return id > other.id;
		}

		constexpr bool operator>=(const EntityUID& other) const noexcept
		{
			return id > other.id;
		}

	private:
		ID_t id{ s_Null };
	};


	using ComponentUID = std::uint32_t;
}