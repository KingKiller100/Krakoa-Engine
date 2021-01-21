#include "Precompile.hpp"
#include "GlobalConfig.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/String/Tricks/kStringOperators.hpp>

#include <filesystem>

namespace krakoa::configuration
{
	namespace
	{
		template<typename T>
		T GetImpl(const std::string& valueStr)
		{
			const auto value = klib::StrTo<T>(valueStr);
			return value;
		}
	}

	GlobalConfig::GlobalConfig()
	{
		Initialize();
	}

	GlobalConfig::~GlobalConfig() noexcept
	{
		parsers.clear();
	}

	void GlobalConfig::Initialize()
	{
		const auto krakoaConfigPath = klib::GetCurrentWorkingDirectory() + "\\..\\Krakoa\\Config\\";

		std::filesystem::directory_iterator dir_iter(krakoaConfigPath);
		std::filesystem::directory_iterator end_iter();

		for (; dir_iter != end_iter(); ++ dir_iter)
		{
			const auto& entry = *dir_iter;
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();

			if (path.has_extension() || path.extension().string().find("ini") == std::string::npos)
				continue;

			const auto pathStr = path.string();
			
			parsers.emplace(pathStr, Make_Solo<ConfigFileParser>(pathStr));
		}
	}

	template <>
	bool GlobalConfig::Get<bool>(const std::string& context, const std::string& key)
	{
		auto& parser = parsers[context.data()];
		const auto& valueStr = parser->RetrieveValue(key.data());
		const auto value = klib::StrTo<size_t>(valueStr);
		return value != 0;
	}

	template <>
	double GlobalConfig::Get<double>(const std::string& context, const std::string& key)
	{
		auto& parser = parsers[context.data()];
		const auto& valueStr = parser->RetrieveValue(key.data());
		const auto value = klib::StrTo<double>(valueStr);
		return value ;
	}
}
