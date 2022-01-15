#include "Precompile.hpp"
#include "MemoryLogger.hpp"

#include "../util/Fmt.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/Logging/kLogging.hpp>
#include <Utility/Logging/Destinations/kFileLogger.hpp>


namespace memory
{
	klib::LogDispatcher MemoryLogger::dispatcher_;
	using namespace klib::kString::operators;

	namespace
	{
		klib::LogDispatcher::LogDestRef<klib::FileLogger> g_FileLogger;
		krakoa::LogProfile g_MemoryLog = MemoryLogger::RegisterProfile( "Memory" );
	}

	void MemoryLogger::Init( std::string_view openingMsg )
	{
		if ( g_MemoryLog.IsNull() )
			return;

		std::filesystem::path dir( klib::kFileSystem::GetExeDirectory() );
		dir /= "Logs";
		constexpr auto filename = "Memory";
		constexpr auto name = filename;
		constexpr auto extension = ".log";

		g_FileLogger = dispatcher_.AddDestination<klib::FileLogger>( ( dir / filename ).string() + std::string( extension ) );

		g_FileLogger->Open();
		g_FileLogger->SetFormat( "[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc]: &t" );
		g_MemoryLog->AddRaw( openingMsg );
	}

	krakoa::LogProfile MemoryLogger::RegisterProfile( std::string_view name )
	{
		return dispatcher_.RegisterProfile( name, klib::LogLevel::INF );
	}

	void MemoryLogger::Raw(std::string_view message)
	{
		g_MemoryLog->AddRaw(message);
	}

	void MemoryLogger::Trace(std::string_view message)
	{
		g_MemoryLog->AddEntry(klib::LogLevel::TRC, message);
	}

	void MemoryLogger::Debug(std::string_view message)
	{
		g_MemoryLog->AddEntry(klib::LogLevel::DBG, message);
	}

	void MemoryLogger::Info(std::string_view message)
	{
		g_MemoryLog->AddEntry(klib::LogLevel::INF, message);
	}

	void MemoryLogger::Warn(std::string_view message)
	{
		g_MemoryLog->AddEntry(klib::LogLevel::WRN, message);
	}

	void MemoryLogger::Error(std::string_view message, klib::SourceInfo sourceInfo)
	{
		g_MemoryLog->AddEntry(klib::LogLevel::ERR, util::Fmt("{0}\n{1}", message, sourceInfo));
	}

	void MemoryLogger::Banner(std::string_view message, std::string_view frontPadding, std::string_view backPadding, std::uint16_t paddingCount)
	{
		g_MemoryLog->AddBanner(message, frontPadding, backPadding, paddingCount);
	}
}
