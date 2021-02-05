#include "Precompile.hpp"
#include "ConfigValueMap.hpp"

#include "../Debug/Debug.hpp"
#include "../Logging/EngineLogger.hpp"

#include <Utility/String/kToString.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::configurations
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

		KRK_LOG("Config", "Parsing config file: " + path.string());
		
		auto lines = klib::ReadFile(path.string());

		size_t index = 0;
		
		for (auto& line : lines)
		{
			klib::SourceInfo source(path.string(), index++, "");
		
			const auto commentPos = line.find(s_CommentToken);

			if (commentPos != std::string::npos)
				line.erase(commentPos);

			if (klib::IsWhiteSpaceOrNull(line))
				continue;

			klib::Remove(line, ' ');
			const auto colonPos = line.find(':');

			if (colonPos == std::string::npos)
			{
				KRK_FATAL(klib::ToString("[Configurations] Bad line - source: {0}", source));
			}

			const auto key = klib::ToLower(line.substr(0, colonPos));
			const auto value = line.substr(colonPos + 1);

			KRK_NRM(klib::ToString("Mapping: [\"{0}\" -> \"{1}\"]", key, value));
			
			values.emplace(key, value);
		}
		
		return values;
	}
}
