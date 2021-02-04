#include "Precompile.hpp"
#include "GlobalConfig.hpp"


namespace krakoa::configuration
{
	GlobalConfig::GlobalConfig(Token, const std::filesystem::path& parentPath)
		: root(parentPath)
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
		constexpr auto configFolder = "Krakoa\\Config";
		
		const auto configPath = root / configFolder;

		const std::filesystem::directory_iterator end_iter;

		for (std::filesystem::directory_iterator dir_iter(configPath); 
			dir_iter != end_iter; 
			++dir_iter)
		{
			const auto& entry = *dir_iter;
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();

			if (!path.has_extension() 
				|| path.extension().string().find("ini") == std::string::npos)
				continue;

			const auto filename = klib::ToLower(path.stem().string());
			
			configMap.emplace(filename, Make_Solo<ConfigValueMap>(path));
		}
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
