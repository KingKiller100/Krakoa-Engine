#include "Precompile.hpp"
#include "GlobalConfig.hpp"

#include "../Logging/EngineLogger.hpp"
#include "../FileSystem/VirtualFileExplorer.hpp"

namespace krakoa::configurations
{
	GlobalConfig::GlobalConfig(Token)
	{
		Initialize();
	}

	GlobalConfig::~GlobalConfig() noexcept
	{
		KRK_PROFILE_FUNCTION();
		configMap.clear();
	}

	void GlobalConfig::AddRemap(const std::string& redirectKey, const std::string& key)
	{
		remapKeys[SanitizeKey(redirectKey)] = SanitizeKey(key);
	}

	void GlobalConfig::Initialize()
	{
		KRK_PROFILE_FUNCTION();

		KRK_LOG("Config", "Parsing config files");

		const auto files = filesystem::VirtualFileExplorer::GetFiles(
			"Config", "ini", filesystem::FileSearchMode::RECURSIVE
		);

		for (const auto& file : files)
		{
			const auto filename = SanitizeKey(file.stem().string());
			const auto valueMap = new ConfigValueMap(file);
			valueMap->Load();
			configMap.emplace(filename, valueMap);
		}
		
		KRK_LOG("Config", "Parsing concluded");
	}

	std::string GlobalConfig::ResolveKey(const std::string& key) const
	{
		const auto sanitizedKey = SanitizeKey(key);
		const auto iter = remapKeys.find(sanitizedKey);
		if (iter != remapKeys.end())
		{
			return iter->second;
		}

		return sanitizedKey;
	}

	std::string GlobalConfig::SanitizeKey(const std::string& key) const
	{
		return klib::ToLower(key);
	}

	const ConfigValueMap& GlobalConfig::GetValueMap(const std::string& context) const
	{
		KRK_PROFILE_FUNCTION();
		return *configMap.at(SanitizeKey(context));
	}

	GlobalConfig& GetGlobalConfig()
	{
		return GlobalConfig::Reference();
	}

	void RemapConfigurationKey(const std::string& redirectKey, const std::string& key)
	{
		auto& gConf = GlobalConfig::Reference();
		gConf.AddRemap(redirectKey, key);
	}
}
