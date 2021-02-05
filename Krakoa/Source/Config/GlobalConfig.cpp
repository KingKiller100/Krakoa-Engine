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

	void GlobalConfig::Initialize()
	{
		KRK_PROFILE_FUNCTION();

		KRK_LOG("Config", "Parsing config files");

		const auto files = filesystem::VirtualFileExplorer::GetFiles(
			"Config", "ini", filesystem::FileSearchMode::RECURSIVE
		);

		for (const auto& file : files)
		{
			const auto filename = klib::ToLower(file.stem().string());

			configMap.emplace(filename, Make_Solo<ConfigValueMap>(file));
		}
		
		KRK_LOG("Config", "Parsing concluded");
	}

	const ConfigValueMap& GlobalConfig::GetValueMap(const std::string& context) const
	{
		KRK_PROFILE_FUNCTION();
		return *configMap.at(klib::ToLower(context));
	}

	GlobalConfig& GetGlobalConfig()
	{
		return GlobalConfig::Reference();
	}
}
