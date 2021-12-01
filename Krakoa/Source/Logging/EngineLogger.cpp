#include "Precompile.hpp"
#include "EngineLogger.hpp"

#include "../Config/GlobalConfig.hpp"

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
	}

	Solo_Ptr<klib::Logging> EngineLogger::pLogger;

	void EngineLogger::CoreInit( const std::string_view& initMsg )
	{
		if ( pLogger )
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

		pLogger = Make_Solo<klib::Logging>();
		g_FileLogger = pLogger->AddDestination<klib::FileLogger>( path );
		g_ConsoleLogger = pLogger->AddDestination<klib::ConsoleLogger>();
		g_DebugOutputWindowLogger = pLogger->AddDestination<klib::DebugOutputWindowLogger>();

		pLogger->SetCacheMode( false );

		g_FileLogger->Open();
		g_ConsoleLogger->Open();
		g_DebugOutputWindowLogger->Open();

		pLogger->AddRaw( padding );
		pLogger->AddRaw( stamp );
		pLogger->AddRaw( initMsg );
		pLogger->AddRaw( padding );

		pLogger->AddRaw();
	}

	void EngineLogger::Raw( std::string_view message )
	{
		pLogger->AddRaw( message );
	}

	void EngineLogger::Trace( std::string_view profile, std::string_view message )
	{
		pLogger->AddEntry( klib::LogLevel::TRC, klib::LogProfile( profile ), klib::LogMessage( message ) );
	}

	void EngineLogger::Debug( std::string_view profile, std::string_view message )
	{
		pLogger->AddEntry( klib::LogLevel::DBG, klib::LogProfile( profile ), klib::LogMessage( message ) );
	}

	void EngineLogger::Normal( std::string_view profile, std::string_view message )
	{
		pLogger->AddEntry( klib::LogLevel::NRM, klib::LogProfile( profile ), klib::LogMessage( message ) );
	}

	void EngineLogger::Info( std::string_view profile, std::string_view message )
	{
		pLogger->AddEntry( klib::LogLevel::INF, klib::LogProfile( profile ), klib::LogMessage( message ) );
	}

	void EngineLogger::Warn( std::string_view profile, std::string_view message )
	{
		pLogger->AddEntry( klib::LogLevel::WRN, klib::LogProfile( profile ), klib::LogMessage( message ) );
	}

	void EngineLogger::Error( std::string_view profile, std::string_view message, klib::SourceInfo sourceInfo )
	{
		pLogger->AddEntry( klib::LogLevel::ERR, klib::LogProfile( profile ), klib::LogMessage( klib::ToString( "{0}\n{1}", message, sourceInfo ) ) );
	}

	void EngineLogger::Banner( const klib::LogMessage& message, const std::string_view& frontPadding, const std::string_view& backPadding, std::uint16_t paddingCount )
	{
		pLogger->AddBanner( message, frontPadding, backPadding, paddingCount );
	}

	void EngineLogger::ShutDown()
	{
		GetLogger().AddRaw( R"(
************************************************************************
                              Logging Concluded
************************************************************************)"
		);
	}

	void EngineLogger::SetMinimumLogLevelUsingConfig()
	{
		const auto logLevelStr = configurations::GetConfiguration<std::string_view>( "Logging", "Level" );
		const auto min_level = klib::LogLevel::FromString( klib::ToUpper( logLevelStr ) );
		pLogger->SetGlobalLevel( min_level );
	}

	void EngineLogger::RemoveIfTooOldFile()
	{
		const auto logPath = g_FileLogger->GetPath();

		const auto entry = std::filesystem::directory_entry( logPath );

		if ( !entry.exists() )
			return;

		g_FileLogger->Close();

		const auto maxBytes = configurations::GetConfiguration<size_t>( "Logging", "MaxBytes" );
		const auto fileSize = entry.file_size();
		const auto tooBig = fileSize > maxBytes;

		const auto maxDays = configurations::GetConfiguration<size_t>( "Logging", "MaxDays" );
		const auto lastWrite = entry.last_write_time();
		const auto now = std::filesystem::_File_time_clock::now();
		const auto timeSinceLastWrite = now - lastWrite;
		const auto totalHours = std::chrono::duration_cast<std::chrono::hours>( timeSinceLastWrite );
		const auto totalDays = totalHours / 24.0;
		const auto tooOld = maxDays < totalDays.count();

		if ( tooBig || tooOld )
			klib::Remove( entry.path() );

		g_FileLogger->Open();
	}

	klib::Logging& EngineLogger::GetLogger()
	{
		return *pLogger;
	}
}
