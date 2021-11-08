#pragma once

#include <cstdint>

namespace util
{
	template<typename Identifier_t>
	struct BasicTypeUniqueIdentifier
	{
	public:
		using ID_t = Identifier_t;

	protected:
		template<typename U>
		static ID_t GetUniqueID()
		{
			static const auto id = MakeUniqueID();
			return id;
		}

	private:
		static auto MakeUniqueID()
		{
			static ID_t id = 0;
			return id++;
		}
	};

	using TypeUniqueIdentifier = BasicTypeUniqueIdentifier<std::uint64_t>;
}

