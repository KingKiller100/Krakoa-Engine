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

		const DataMap& Retrieve() const;
		const DataMap::mapped_type& RetrieveValue(
			const DataMap::key_type& key) const;

	private:
		void Read(const std::filesystem::path& path);
		
	private:
		std::unordered_map<std::string, std::string> values;
	};
}
