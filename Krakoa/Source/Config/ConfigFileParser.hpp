#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::configuration
{
	class ConfigFileParser
	{
		using DataMap = std::unordered_map<std::string, std::string>;
		
	public:
		ConfigFileParser(const std::filesystem::path& path) noexcept;

		~ConfigFileParser() noexcept;

		const ConfigFileParser::DataMap& Retrieve() const;
		const std::unordered_map<std::string, std::string>::mapped_type& RetrieveValue(
			const DataMap::key_type& key) const;
	private:
		bool Read(const std::filesystem::path& path);
		bool SkipLine(const std::string& line);
		
	private:
		std::filesystem::path filepath;
		std::unordered_map<std::string, std::string> values;
	};
}
