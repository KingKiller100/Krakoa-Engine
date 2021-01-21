#include "Precompile.hpp"
#include "ConfigFileParser.hpp"

#include "../Logging/EngineLogger.hpp"

#include <Utility/String/kToString.hpp>
#include <Utility/String/kStringTricks.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::configuration
{
	ConfigFileParser::ConfigFileParser(const std::filesystem::path& path) noexcept
	{
		KRK_DBG("Attempting to read config file: " + path.string());
		
		const auto success = Read(path);

		KRK_DBG(klib::ToString("Parse successful: {0}", success));
	}

	ConfigFileParser::~ConfigFileParser() noexcept
	{
		values.clear();
	}

	const ConfigFileParser::DataMap& ConfigFileParser::Retrieve() const
	{
		return values;
	}

	const std::unordered_map<std::string, std::string>::mapped_type& ConfigFileParser::RetrieveValue(
		const DataMap::key_type& key) const
	{
		return values.at(key);
	}

	bool ConfigFileParser::Read(const std::filesystem::path& path)
	{
		auto lines = klib::ReadFile(path.string());

		for (auto& line: lines)
		{
			if (SkipLine(line))
				continue;
			
			klib::Remove(line, ' ');
			const auto colonPos = line.find(':');

			if (colonPos == std::string::npos)
				return false;
			
			const auto key = line.substr(0, colonPos);
			const auto value = line.substr(colonPos + 1);

			KRK_INF(klib::ToString("Key: \"{0}\" Value: \"{1}\"", key, value));
			
			values.emplace(key, value);
		}

		return true;
	}

	bool ConfigFileParser::SkipLine(const std::string& line)
	{
		return line.empty() || line.front() == '*';
	}
}
