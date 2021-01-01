#pragma once

#include <cstdint>

namespace util
{
	template<typename T, typename Identifier_t = std::uint64_t>
	struct TypeUniqueIdentifier
	{
	public:
		using ID_t = Identifier_t;
		
	private:
		static auto MakeUniqueID()
		{
			static ID_t id = 0;
			return id++;
		}

	protected:
		template<typename U>
		static ID_t GetUniqueID()
		{
			static const auto id = MakeUniqueID();
			return id;
		}
	};
	
	template<typename Identifier_t = std::uint64_t >
	using TypeUID = TypeUniqueIdentifier<void, Identifier_t>;

	
}

