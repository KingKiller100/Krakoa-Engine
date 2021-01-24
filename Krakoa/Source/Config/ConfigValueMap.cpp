#include "Precompile.hpp"
#include "ConfigValueMap.hpp"

#include <Utility/String/kStringTricks.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::configuration
{
	ConfigValueMap::ConfigValueMap(const std::filesystem::path& path) noexcept
		: values(ConfigFileParser::ParseFile(path))
	{}

	ConfigValueMap::~ConfigValueMap() noexcept
	{
		KRK_PROFILE_FUNCTION();
		values.clear();
	}

	const ConfigValueMap::DataMap& ConfigValueMap::RetrieveMap() const
	{
		
		return values;
	}

	const ConfigValueMap::DataMap::mapped_type& ConfigValueMap::RetrieveValue(
		const DataMap::key_type& key) const
	{
		return values.at(klib::ToLower(key.data()));
	}

	ValueMap ConfigFileParser::ParseFile(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
		
		ValueMap values;
		
		auto lines = klib::ReadFile(path.string());

		for (auto& line : lines)
		{
			const auto commentPos = line.find(s_CommentToken);

			if (commentPos != std::string::npos)
				line.erase(commentPos);

			if (klib::IsWhiteSpaceOrNull(line))
				continue;

			klib::Remove(line, ' ');
			const auto colonPos = line.find(':');

			if (colonPos == std::string::npos)
			{
				throw std::runtime_error("Bad line: \"" + line +
					"\" source: \"" + path.string() + "\"");
			}

			const auto key = klib::ToLower(line.substr(0, colonPos));
			const auto value = line.substr(colonPos + 1);

			values.emplace(key, value);
		}
		
		return values;
	}
}
