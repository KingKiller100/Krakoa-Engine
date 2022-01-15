#include "Precompile.hpp"
#include "ConfigValueMap.hpp"

#include <Utility/String/kToString.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::configurations
{
	ValueMap ConfigFileParser::ParseFile(const std::filesystem::path& path)
	{
		return filesystem::IniFile::ReadFile(path);
	}

	ConfigValueMap::ConfigValueMap(const std::filesystem::path& path) noexcept
		: values()
		, path(path)
	{}

	ConfigValueMap::~ConfigValueMap() noexcept
	{
		KRK_PROFILE_FUNCTION();
		values.clear();
	}

	void ConfigValueMap::Load()
	{
		values = ConfigFileParser::ParseFile(path);
	}

	const ValueMap& ConfigValueMap::RetrieveMap() const
	{
		return values;
	}

	const ValueMap::mapped_type& ConfigValueMap::RetrieveValue(
		const ValueMap::key_type& key) const
	{
		return values.at(SanitizeKey(key).data());
	}

	void ConfigValueMap::Set(const std::string& key, const std::string& value, const klib::MutSourceInfo& source)
	{
		values[SanitizeKey(key)] = ValueMap::mapped_type{ value, source };
	}

	std::string ConfigValueMap::SanitizeKey(const std::string& key) const
	{
		return klib::ToLower(key);
	}
}
