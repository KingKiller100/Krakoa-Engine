#pragma once

#include "../Debug/Instrumentor.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::configurations
{
	using ValueMap = std::unordered_map<std::string, std::string>;
	
	class ConfigFileParser
	{
	protected:
		static constexpr auto s_CommentToken = '*';
		
	public:
		static ValueMap ParseFile(const std::filesystem::path& path);
	};
	
	class ConfigValueMap
	{
		using DataMap = std::unordered_map<std::string, std::string>;
		
	public:
		ConfigValueMap(const std::filesystem::path& path) noexcept;

		~ConfigValueMap() noexcept;

		template<typename T>
		T ReadAs(const std::string& key) const
		{
			KRK_PROFILE_FUNCTION();
			
			const auto& valueStr = RetrieveValue(klib::ToLower(key));

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
				T value;
				if constexpr (klib::type_trait::Is_Bool_V<T>)
				{
					value = klib::StrTo<std::uintptr_t>(valueStr) != 0;
				}
				else
				{
					value = klib::StrTo<T>(valueStr);
				}
				return value;
			}
		}

		const DataMap& RetrieveMap() const;
		const DataMap::mapped_type& RetrieveValue(
			const DataMap::key_type& key) const;
		
	private:
		std::unordered_map<std::string, std::string> values;
	};
}
