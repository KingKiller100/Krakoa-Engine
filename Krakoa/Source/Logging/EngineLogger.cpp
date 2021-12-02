#include "Precompile.hpp"
#include "EngineLogger.hpp"

#include "../Config/GlobalConfig.hpp"
#include "../Util/Fmt.hpp"

#include "../Core/PointerTypes.hpp"

#include <Utility/Logging/kLogging.hpp>
#include <Utility/Logging/Destinations/kFileLogger.hpp>
#include <Utility/Logging/Destinations/kConsoleLogger.hpp>
#include <Utility/Logging/Destinations/kDebugOutputWindowLogger.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/Calendar/kCalendar.hpp>

namespace krakoa
{
	using namespace filesystem;

	namespace
	{
		std::shared_ptr<klib::FileLogger> g_FileLogger;
		std::shared_ptr<klib::ConsoleLogger> g_ConsoleLogger;
		std::shared_ptr<klib::DebugOutputWindowLogger> g_DebugOutputWindowLogger;
		Solo_Ptr<klib::Logging> g_Logger;
	}


	void EngineLogger::CoreInit( const std::string_view& initMsg )
	{
		if ( g_Logger )
			return;

		std::filesystem::path dir( klib::kFileSystem::GetExeDirectory() );
		dir /= "Logs\\";

		constexpr auto name = "Krakoa";
		constexpr auto filename = "KrakoaEngine";
		constexpr auto extension = ".log";

		const auto path = dir / ( std::string( filename ) + extension );

		const auto padding = std::string( 72, '*' );
		const auto spacing = std::string( 12, ' ' );

		const klib::GregorianCalendar now( klib::CalendarInfoSourceType::LOCAL );
		const auto timeStr = now.GetTime().ToString( klib::Time::MILLIS );
		const auto dateStr = now.GetDate().ToString( "mmm ddd yyyy" );
		const auto stamp = spacing + dateStr + spacing + timeStr + spacing;

		g_Logger = Make_Solo<klib::Logging>();
		g_FileLogger = g_Logger->AddDestination<klib::FileLogger>( path );
		g_ConsoleLogger = g_Logger->AddDestination<klib::ConsoleLogger>();
		g_DebugOutputWindowLogger = g_Logger->AddDestination<klib::DebugOutputWindowLogger>();

		g_Logger->SetCacheMode( false );

		g_FileLogger->Open();
		g_ConsoleLogger->Open();
		g_DebugOutputWindowLogger->Open();

		g_Logger->AddRaw( padding );
		g_Logger->AddRaw( stamp );
		g_Logger->AddRaw( initMsg );
		g_Logger->AddRaw( padding );

		g_Logger->AddRaw();
	}

	void EngineLogger::Raw( std::string_view message )
	{
		g_Logger->AddRaw( message );
	}

	void EngineLogger::Trace( std::string_view profile, std::string_view message )
	{
		g_Logger->AddEntry( klib::LogLevel::TRC, klib::LogProfile( profile ), klib::LogMessage( message ) );
	}

	void EngineLogger::Debug( std::string_view profile, std::string_view message )
	{
		g_Logger->AddEntry( klib::LogLevel::DBG, klib::LogProfile( profile ), klib::LogMessage( message ) );
	}

	void EngineLogger::Normal( std::string_view profile, std::string_view message )
	{
		g_Logger->AddEntry( klib::LogLevel::NRM, klib::LogProfile( profile ), klib::LogMessage( message ) );
	}

	void EngineLogger::Info( std::string_view profile, std::string_view message )
	{
		g_Logger->AddEntry( klib::LogLevel::INF, klib::LogProfile( profile ), klib::LogMessage( message ) );
	}

	void EngineLogger::Warn( std::string_view profile, std::string_view message )
	{
		g_Logger->AddEntry( klib::LogLevel::WRN, klib::LogProfile( profile ), klib::LogMessage( message ) );
	}

	void EngineLogger::Error( std::string_view profile, std::string_view message, klib::SourceInfo sourceInfo )
	{
		g_Logger->AddEntry( klib::LogLevel::ERR, klib::LogProfile( profile ), klib::LogMessage( klib::ToString( "{0}\n{1}", message, sourceInfo ) ) );
	}

	void EngineLogger::Banner( const klib::LogMessage& message, const std::string_view& frontPadding, const std::string_view& backPadding, std::uint16_t paddingCount )
	{
		g_Logger->AddBanner( message, frontPadding, backPadding, paddingCount );
	}

	void EngineLogger::ShutDown()
	{
		g_Logger->AddRaw( R"(
************************************************************************
                              Logging Concluded
************************************************************************)"
		);
	}

	void EngineLogger::SetMinimumLogLevelUsingConfig( klib::LogLevel minLvl )
	{
		// const auto logLevelStr = configurations::GetConfiguration<std::string_view>( "Logging", "Level" );
		// const auto min_level = klib::LogLevel::FromString( klib::ToUpper( logLevelStr ) );
		g_Logger->SetGlobalLevel( minLvl );
	}

	void EngineLogger::RemoveIfTooOldFile( std::int64_t maxDays )
	{
		const auto dirEntry = std::filesystem::directory_entry( g_FileLogger->GetPath() );

		if ( !dirEntry.exists() )
			return;

		g_FileLogger->Close();
		
		const auto lastWrite = dirEntry.last_write_time();

		const auto now = std::chrono::file_clock::now();
		const auto timeSinceLastWrite = now - lastWrite;
		const auto totalHours = std::chrono::duration_cast<std::chrono::hours>( timeSinceLastWrite );
		const auto totalDays = totalHours / 24.0;
		const auto tooOld = maxDays < totalDays.count();

		if ( tooOld )
			klib::Remove( dirEntry.path() );

		g_FileLogger->Open();
	}

	void EngineLogger::RemoveIfTooLarge(std::uint64_t maxBytes)
	{
		const auto dirEntry = std::filesystem::directory_entry(g_FileLogger->GetPath());

		if (!dirEntry.exists())
			return;

		g_FileLogger->Close();

		const auto fileSize = dirEntry.file_size();
		const auto tooBig = fileSize > maxBytes;

		if (tooBig )
			klib::Remove(dirEntry.path());

		g_FileLogger->Open();
	}
}
