#pragma once

#include "ConfigValueMap.hpp"

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
		using ConfigMap = std::unordered_map<std::string, Solo_Ptr<ConfigValueMap>>;
		
	public:
		GlobalConfig(Token, const std::filesystem::path& parentPath);
		~GlobalConfig() noexcept;

		template<typename T>
		T Get(const std::string& context, const std::string& key) const
		{
			const auto& valueMap = GetValueMap(klib::ToLower(context));
			const auto& value = valueMap.ReadAs<T>(key);
			return value;
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

		const ConfigValueMap& GetValueMap(const std::string& context) const;
		
	private:
		void Initialize();

	private:
		std::filesystem::path root;
		ConfigMap configMap;
	};

	GlobalConfig& GetGlobalConfig();
}