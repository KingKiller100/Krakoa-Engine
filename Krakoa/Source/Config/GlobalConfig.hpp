#pragma once

#include "ConfigFileParser.hpp"

#include "../Patterns/ManagerBase.hpp"
#include "../Core/PointerTypes.hpp"

#include <Utility/String/kStringTricks.hpp>

#include <string>
#include <unordered_map>

namespace krakoa::configuration
{
	class GlobalConfig : public patterns::ManagerBase<GlobalConfig>
	{
	public:
		GlobalConfig();
		~GlobalConfig() noexcept;

		void Initialize();

		template<typename T>
		T Get(const std::string& context, const std::string& key)
		{
			auto& parser = parsers[context.data()];
			const auto& valueStr = parser->RetrieveValue(key.data());
			const auto value = klib::StrTo<T>(valueStr);
			return value;
		}
		
	private:
		std::unordered_map<std::string, Solo_Ptr<ConfigFileParser>> parsers;
	};
}