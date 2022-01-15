#include "Precompile.hpp"
#include "EngineLogger.hpp"

#include "../Config/GlobalConfig.hpp"

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
		klib::LogDispatcher::LogDestRef<klib::FileLogger> g_FileLogger;
		klib::LogDispatcher::LogDestRef<klib::ConsoleLogger> g_ConsoleLogger;
		klib::LogDispatcher::LogDestRef<klib::DebugOutputLogger> g_DebugOutputLogger;
		LogProfile g_KrakoaLog = EngineLogger::RegisterProfile( "Krakoa");
	}


	void EngineLogger::CoreInit( std::string_view initMsg )
	{
		if ( g_KrakoaLog.IsNull() )
			return;

		std::filesystem::path dir( klib::kFileSystem::GetExeDirectory() );
		dir /= "Logs\\";
		
		constexpr auto filename = "KrakoaEngine";
		constexpr auto extension = ".log";

		const auto path = dir / ( std::string( filename ) + extension );

		const auto padding = std::string( 72, '*' );
		const auto spacing = std::string( 12, ' ' );

		const klib::GregorianCalendar today( klib::CalendarInfoSourceType::LOCAL );
		const auto timeStr = today.GetTime().ToString( klib::Time::MILLIS );
		const auto dateStr = today.GetDate().ToString( "mmm ddd yyyy" );
		const auto stamp = spacing + dateStr + spacing + timeStr + spacing;
		
		g_FileLogger = dispatcher_.AddDestination<klib::FileLogger>( path );
		g_ConsoleLogger = dispatcher_.AddDestination<klib::ConsoleLogger>();
		g_DebugOutputLogger = dispatcher_.AddDestination<klib::DebugOutputLogger>();
		
		dispatcher_.Open();

		g_KrakoaLog->AddRaw( padding );
		g_KrakoaLog->AddRaw( stamp );
		g_KrakoaLog->AddRaw( initMsg );
		g_KrakoaLog->AddRaw( padding );

		g_KrakoaLog->AddNewLine();
	}

	void EngineLogger::Raw( std::string_view message )
	{
		g_KrakoaLog->AddRaw( message );
	}

	void EngineLogger::Trace( std::string_view message )
	{
		g_KrakoaLog->AddEntry( klib::LogLevel::TRC, message );
	}

	void EngineLogger::Debug( std::string_view message )
	{
		g_KrakoaLog->AddEntry( klib::LogLevel::DBG, message );
	}

	void EngineLogger::Info( std::string_view message )
	{
		g_KrakoaLog->AddEntry( klib::LogLevel::INF, message );
	}

	void EngineLogger::Warn( std::string_view message )
	{
		g_KrakoaLog->AddEntry( klib::LogLevel::WRN, message );
	}

	void EngineLogger::Error( std::string_view message, klib::SourceInfo sourceInfo )
	{
		g_KrakoaLog->AddEntry( klib::LogLevel::ERR, klib::ToString( "{0}\n{1}", message, sourceInfo ) );
	}

	void EngineLogger::Banner( std::string_view message, std::string_view frontPadding, std::string_view backPadding, std::uint16_t paddingCount )
	{
		g_KrakoaLog->AddBanner( message, frontPadding, backPadding, paddingCount );
	}

	void EngineLogger::ShutDown()
	{
		g_KrakoaLog->AddRaw( R"(
************************************************************************
                              Logging Concluded
************************************************************************)"
		);
	}

	LogProfile EngineLogger::RegisterProfile( std::string_view name )
	{
		return dispatcher_.RegisterProfile( name, klib::LogLevel::INF );
	}

	void EngineLogger::SetMinimumLogLevelUsingConfig( klib::LogLevel minLvl )
	{
		dispatcher_.SetGlobalLevel( minLvl );
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

	void EngineLogger::RemoveIfTooLarge( std::uint64_t maxBytes )
	{
		const auto dirEntry = std::filesystem::directory_entry( g_FileLogger->GetPath() );

		if ( !dirEntry.exists() )
			return;

		g_FileLogger->Close();

		const auto fileSize = dirEntry.file_size();
		const auto tooBig = fileSize > maxBytes;

		if ( tooBig )
			klib::Remove( dirEntry.path() );

		g_FileLogger->Open();
	}
}
