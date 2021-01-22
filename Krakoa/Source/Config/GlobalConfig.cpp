﻿#include "Precompile.hpp"
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
		parsers.clear();
	}

	void GlobalConfig::Initialize()
	{
		constexpr auto configFolder = "Config";
		
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
			
			parsers.emplace(filename, Make_Solo<ConfigFileParser>(path));
		}
	}
}