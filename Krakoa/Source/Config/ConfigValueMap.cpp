#include "Precompile.hpp"
#include "ConfigValueMap.hpp"

#include "../Debug/Debug.hpp"
#include "../Logging/EngineLogger.hpp"

#include <Utility/String/kToString.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::configurations
{
	ValueMap ConfigFileParser::ParseFile(const std::filesystem::path& path)
	{
		return filesystem::IniFile::ReadFile(path);
	}

	ConfigValueMap::ConfigValueMap(const std::filesystem::path& path) noexcept
		: values(ConfigFileParser::ParseFile(path))
	{}

	ConfigValueMap::~ConfigValueMap() noexcept
	{
		KRK_PROFILE_FUNCTION();
		values.clear();
	}

	const ValueMap& ConfigValueMap::RetrieveMap() const
	{
		return values;
	}

	const ValueMap::mapped_type& ConfigValueMap::RetrieveValue(
		const ValueMap::key_type& key) const
	{
		return values.at(klib::ToLower(key.data()));
	}
}
