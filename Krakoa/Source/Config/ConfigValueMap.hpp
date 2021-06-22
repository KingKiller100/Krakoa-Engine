#pragma once

#include "../Debug/Instrumentor.hpp"
#include "../FileSystem/IniFile.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::configurations
{
	using ValueMap = filesystem::IniFile::ValueMap;

	class ConfigFileParser
	{
	public:
		static ValueMap ParseFile(const std::filesystem::path& path);
	};

	class ConfigValueMap
	{
	public:
		ConfigValueMap(const std::filesystem::path& path) noexcept;

		~ConfigValueMap() noexcept;

		template<typename T>
		T ReadAs(const std::string& key) const
		{
			KRK_PROFILE_FUNCTION();

			const auto& valueStr = RetrieveValue(SanitizeKey(key)).value;

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

		void Load();
		const ValueMap& RetrieveMap() const;
		const ValueMap::mapped_type& RetrieveValue(
			const ValueMap::key_type& key) const;

		void Set(const std::string& key, const std::string& value, const klib::MutSourceInfo& source);
		std::string SanitizeKey(const std::string& key) const;

	private:
		ValueMap values;
		std::filesystem::path path;
	};
}
