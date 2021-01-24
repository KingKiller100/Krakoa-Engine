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
	class EntityUID : klib::kTemplate::SimpleOperators<EntityUID>
	{
		static constexpr auto s_Null = std::numeric_limits<std::uint64_t>::max();
	public:
		using ID_t = std::uint64_t;

	public:
		constexpr EntityUID(std::uint64_t val = s_Null) noexcept
			: id(val)
		{}

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

		ID_t GetValue() const;
		
		template<typename T>
		constexpr operator T() const noexcept
		{
			return static_cast<T>(GetValue());
		}
		
		template<typename T>
		constexpr operator T*() const noexcept
		{
			return (T*)GetValue();
		}
		
		EntityUID Mask() const;
		
		[[nodiscard]] std::string ToString() const noexcept;
		
	private:
		std::uint64_t id;
	};


	using ComponentUID = std::uint32_t;
}