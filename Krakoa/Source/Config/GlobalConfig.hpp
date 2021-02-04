#pragma once

#include "ConfigValueMap.hpp"

#include "../Patterns/ManagerBase.hpp"
#include "../Core/PointerTypes.hpp"

#include <Utility/String/kStringTricks.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::configurations
{
	class GlobalConfig : public patterns::ManagerBase<GlobalConfig>
	{
		using ConfigMap = std::unordered_map<std::string, Solo_Ptr<ConfigValueMap>>;

	public:
		GlobalConfig(Token);
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
		ConfigMap configMap;
	};

	GlobalConfig& GetGlobalConfig();

	template<typename T>
	T GetConfiguration(const std::string& context, const std::string& key)
	{
		return GetGlobalConfig().Get<T>(context, key);
	}

	template<typename T>
	T GetConfiguration(const std::string& context, const std::string& key, T defaultValue)
	{
		return GetGlobalConfig().TryGet<T>(context, key, defaultValue);
	}
}