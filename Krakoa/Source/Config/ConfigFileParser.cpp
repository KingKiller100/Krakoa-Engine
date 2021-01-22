#include "Precompile.hpp"
#include "ConfigFileParser.hpp"

#include <Utility/String/kStringTricks.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::configuration
{
	ConfigFileParser::ConfigFileParser(const std::filesystem::path& path) noexcept
	{
		Read(path);
	}

	ConfigFileParser::~ConfigFileParser() noexcept
	{
		values.clear();
	}

	const ConfigFileParser::DataMap& ConfigFileParser::Retrieve() const
	{
		return values;
	}

	const ConfigFileParser::DataMap::mapped_type& ConfigFileParser::RetrieveValue(
		const DataMap::key_type& key) const
	{
		return values.at(key);
	}

	void ConfigFileParser::Read(const std::filesystem::path& path)
	{
		auto lines = klib::ReadFile(path.string());

		for (auto& line : lines)
		{
			if (line.empty() || line.front() == '*')
				continue;

			klib::Remove(line, ' ');
			const auto colonPos = line.find(':');

			if (colonPos == std::string::npos)
			{
				throw std::runtime_error("Bad line: \"" + line +
					"\" source: \"" + path.string() + "\"");
			}
			
			const auto key =  klib::ToLower(line.substr(0, colonPos));
			const auto value = line.substr(colonPos + 1);

			values.emplace(key, value);
		}
	}
}
