#pragma once

#include "../Instrumentor.hpp"

#include <cstdint>

namespace util
{
	template<typename T>
	struct UniqueIdentifier
	{
	protected:
		using ID_t = std::uint64_t;

	private:
		static auto MakeUniqueID()
		{
			static ID_t id = 0;
			return id++;
		}

	protected:
		template<typename T>
		static ID_t GetUniqueID()
		{
			KRK_PROFILE_FUNCTION();
			static const auto id = MakeUniqueID();
			return id;
		}
	};
}

