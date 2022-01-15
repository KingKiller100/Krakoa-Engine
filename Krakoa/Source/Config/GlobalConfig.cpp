#include "Precompile.hpp"
#include "GlobalConfig.hpp"

#include "../Logging/EngineLogger.hpp"
#include "../FileSystem/VirtualFileExplorer.hpp"

namespace krakoa::configurations
{
	LogProfile g_ConfigLog = EngineLogger::RegisterProfile( "Config" );

	GlobalConfig::GlobalConfig( Token )
	{
		Initialize();
	}

	GlobalConfig::~GlobalConfig() noexcept
	{
		KRK_PROFILE_FUNCTION();
		configMap.clear();
	}

	void GlobalConfig::AddContextRemap( const std::string& redirectKey, const std::string& key )
	{
		contextKeyRemaps[SanitizeKey( redirectKey )] = SanitizeKey( key );
	}

	void GlobalConfig::AddSubjectRemap( const std::string& redirectKey, const std::string& key )
	{
		subjectKeyRemaps[SanitizeKey( redirectKey )] = SanitizeKey( key );
	}

	void GlobalConfig::Initialize()
	{
		KRK_PROFILE_FUNCTION();

		g_ConfigLog->AddEntry( klib::LogLevel::INF, "Parsing config files" );

		const auto files = filesystem::VirtualFileExplorer::GetFiles(
			"Config", "ini", filesystem::FileSearchMode::RECURSIVE
		);

		for ( const auto& file : files )
		{
			const auto filename = SanitizeKey( file.stem().string() );
			const auto valueMap = new ConfigValueMap( file );
			valueMap->Load();
			configMap.emplace( filename, valueMap );
		}

		g_ConfigLog->AddEntry( klib::LogLevel::INF, "Parsing concluded" );
	}

	std::string GlobalConfig::ResolveKey( const std::string& key, const std::map<std::string, std::string>& remap ) const
	{
		KRK_PROFILE_FUNCTION();
		const auto sanitizedKey = SanitizeKey( key );
		const auto iter = remap.find( sanitizedKey );
		if ( iter != remap.end() )
		{
			return iter->second;
		}

		return sanitizedKey;
	}

	std::string GlobalConfig::SanitizeKey( const std::string& key ) const
	{
		return klib::ToLower( key );
	}

	void GlobalConfig::LogNewConfiguration( std::string_view context, std::string_view key, std::string_view value, const klib::MutSourceInfo& sourceInfo )
	{
		g_ConfigLog->AddEntry( klib::LogLevel::INF, util::Fmt( "Setting configuration: \"{0}\": [\"{1}\", {2}]", context, key, value ) );
		g_ConfigLog->AddEntry( klib::LogLevel::INF, util::Fmt( "Source: {0}", sourceInfo ) );
	}

	const ConfigValueMap& GlobalConfig::GetValueMap( const std::string& context ) const
	{
		KRK_PROFILE_FUNCTION();
		return *configMap.at( SanitizeKey( context ) );
	}

	GlobalConfig& GetGlobalConfig()
	{
		return GlobalConfig::Reference();
	}

	void RemapConfigurationKey( const std::string& redirectKey, const std::string& key )
	{
		auto& gConf = GlobalConfig::Reference();
		gConf.AddContextRemap( redirectKey, key );
	}
}
