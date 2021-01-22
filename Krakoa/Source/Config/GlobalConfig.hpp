#pragma once

#include "ConfigFileParser.hpp"

#include "../Patterns/ManagerBase.hpp"
#include "../Core/PointerTypes.hpp"

#include <Utility/String/kStringTricks.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::configuration
{
	class GlobalConfig : public patterns::ManagerBase<GlobalConfig>
	{
	public:
		GlobalConfig(Token, const std::filesystem::path& parentPath);
		~GlobalConfig() noexcept;

		template<typename T>
		T Get(const std::string& context, const std::string& key) const
		{
			auto& parser = parsers.at(klib::ToLower(context));
			const auto& valueStr = parser->RetrieveValue(klib::ToLower(key));

			if constexpr (klib::type_trait::Is_String_V<T>)
			{
				return valueStr;
			}
			else if constexpr (klib::type_trait::Is_CString_V<T>)
			{
				return valueStr.data();
			}
			else
			{
				const auto value = klib::StrTo<T>(valueStr);
				if constexpr (klib::type_trait::Is_Bool_V<T>)
				{
					return value != 0;
				}
				else
				{
					return value;
				}
			}
		}

		template<typename T>
		T TryGet(const std::string& context, const std::string& key, T defaultValue) const
		{
			try
			{
				return Get<T>(context, key);
			}
			catch (...)
			{
				return defaultValue;
			}
		}

	private:
		void Initialize();

	private:
		std::filesystem::path root;
		std::unordered_map<std::string, Solo_Ptr<ConfigFileParser>> parsers;
	};
}