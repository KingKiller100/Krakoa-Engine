#include "Precompile.hpp"
#include "ECS_UID.hpp"

#include <Utility/String/kToString.hpp>

namespace krakoa::scene::ecs
{
	EntityUID::ID_t EntityUID::operator++(int)
	{
		return id++;
	}

	EntityUID::ID_t EntityUID::operator++()
	{
		return ++id;
	}

	EntityUID::ID_t EntityUID::operator--(int)
	{
		return id--;
	}

	EntityUID::ID_t EntityUID::operator--()
	{
		return --id;
	}


	EntityUID::ID_t EntityUID::GetValue() const
	{
		return id;
	}

	EntityUID EntityUID::Mask() const
	{
		return id | (1ull << 63);
	}

	std::string EntityUID::ToString() const noexcept
	{
		return klib::ToString<char>(klib::tags::NoFormatTag{}, id);
	}
}
