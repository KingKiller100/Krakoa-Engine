#include "Precompile.hpp"
#include "GlobalConfig.hpp"

#include <filesystem>

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
		KRK_BANNER("Reading configuration files", "Global Config", "*", "*", 30);
		
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

			const auto filename = path.stem().string();
			
			parsers.emplace(filename, Make_Solo<ConfigFileParser>(path));
		}
			
		KRK_BANNER("Done", "Global Config", "*", "*", 30);
	}
	//
	// template <>
	// bool GlobalConfig::Get<bool>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<size_t>(valueStr);
	// 	return value != 0;
	// }
	//
	// template <>
	// std::uint8_t GlobalConfig::Get<std::uint8_t>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<std::uint8_t>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// std::uint16_t GlobalConfig::Get<std::uint16_t>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<std::uint16_t>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// std::uint32_t GlobalConfig::Get<std::uint32_t>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<std::uint32_t>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// std::uint64_t GlobalConfig::Get<std::uint64_t>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<std::uint64_t>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// std::int8_t GlobalConfig::Get<std::int8_t>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<std::int8_t>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// std::int16_t GlobalConfig::Get<std::int16_t>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<std::int16_t>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// std::int32_t GlobalConfig::Get<std::int32_t>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<std::int32_t>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// std::int64_t GlobalConfig::Get<std::int64_t>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<std::int64_t>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// long GlobalConfig::Get<long>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<long>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// unsigned long GlobalConfig::Get<unsigned long>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<unsigned long>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// float GlobalConfig::Get<float>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<float>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// double GlobalConfig::Get<double>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<double>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// long double GlobalConfig::Get<long double>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	const auto value = klib::StrTo<long double>(valueStr);
	// 	return value;
	// }
	//
	// template <>
	// std::string GlobalConfig::Get<std::string>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	return valueStr;
	// }
	//
	// template <>
	// std::string_view GlobalConfig::Get<std::string_view>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	return valueStr;
	// }
	//
	// template <>
	// const char* GlobalConfig::Get<const char*>(const std::string& context, const std::string& key)
	// {
	// 	auto& parser = parsers[context.data()];
	// 	const auto& valueStr = parser->RetrieveValue(key.data());
	// 	return valueStr.data();
	// }
}
