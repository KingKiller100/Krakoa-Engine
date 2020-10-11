#pragma once

#include "../Instrumentor.hpp"

#include <cstdint>

namespace util
{
	namespace secret::helper
	{
		inline auto MakeUniqueID()
		{
			static std::uint64_t id = 0;
			return id++;
		}
	}

	template<typename T>
	static std::uint64_t GetUniqueID()
	{
		KRK_PROFILE_FUNCTION();
		static const auto id = secret::helper::MakeUniqueID();
		return id;
	}
}

