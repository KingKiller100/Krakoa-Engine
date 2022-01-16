#pragma once

#ifdef KRAKOA_TEST
#	include "Tests/TestDriver.hpp"
#else
#	include "Application.hpp"
#	include "PointerTypes.hpp"

#	include "../Logging/EngineLogger.hpp"
#	include "../Debug/Instrumentor.hpp"
#	include "../Debug/ExceptionsHandler.hpp"
#	include "../Config/GlobalConfig.hpp"
#	include "../Platform/OperatingSystem/iOperatingSystem.hpp"

#	include "../FileSystem/VirtualFileExplorer.hpp"

#	include <Utility/Debug/kDebugger.hpp>

#	include <memory>
#	include <filesystem>

namespace krakoa
{
	extern void CreateApplication();
}
#endif

#include <iostream>

#include "../Memory/MemoryStructures/HeapFactory.hpp"

#include <Utility/Localization/kLocale.hpp>
#include <Utility/Calendar/kUseCalendarSourceInfo.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

void Launch();
void InitializeVirtualFileSystem();
void InitializeGlobalConfig( const krakoa::os::iOperatingSystem& os );
void TryRunApplication();
void RunApplication();
krakoa::os::iOperatingSystem& ConnectOS();
void ConfigureLogging();

int main()
{
	klib::kLocale::SetDefaultLocale();
	klib::kCalendar::UsePlatformCalendarInfoSource();

#ifdef KRAKOA_DEBUG
	memory::HeapFactory::Initialize();
#endif

	Launch();

#ifdef KRAKOA_DEBUG
	memory::HeapFactory::ReportMemoryLeaks();
	memory::HeapFactory::ShutDown();
#endif

	std::cin.get();

	return EXIT_SUCCESS;
}

void Launch()
{
#ifdef KRAKOA_TEST
	krakoa::tests::TestDriver::Initialize();
	krakoa::tests::TestDriver::RunAll();
	krakoa::tests::TestDriver::ShutDown();
#else

	KRK_INIT_LOGS( "" );

	if ( klib::WaitForDebugger( "RemoteDebugger" ) )
	{
		RunApplication();
	}
	else
	{
		TryRunApplication();
	}

	krakoa::Application::Destroy();
	krakoa::configurations::GlobalConfig::Destroy();
	krakoa::os::DestroyOperatingSystemInfo();
	KRK_LOG_END();
}

void InitializeVirtualFileSystem()
{
	const std::filesystem::path cwd = klib::GetCurrentWorkingDirectory() + "..\\";
	krakoa::filesystem::VirtualFileExplorer::Initialize( cwd );
	krakoa::filesystem::VirtualFileExplorer::Mount( "Krakoa\\Config", "Config" );
	krakoa::filesystem::VirtualFileExplorer::Mount( "Keditor\\Assets", "Assets" );
}

void InitializeGlobalConfig( const krakoa::os::iOperatingSystem& os )
{
	using namespace krakoa::configurations;
	constexpr char appConfigKey[] = "Application";
	constexpr char configKey[] = "OS";

	const auto& sysEnv = os.GetEnvironmentVariables();
	const auto envKeys = sysEnv.GetKeys();

	GlobalConfig::Create();

	SetConfiguration( appConfigKey, "CmdArguments", __argc, MUT_SRC_INFO() );
	for ( auto i = 0; i < __argc; ++i )
	{
		SetConfiguration( appConfigKey, util::Fmt( "CmdArg{0}", i ), __argv[i], MUT_SRC_INFO() );
	}
	SetConfiguration( appConfigKey, "Configuration", klib::GetRuntimeConfigurationStr(), MUT_SRC_INFO() );

	for ( auto&& key : envKeys )
	{
		SetConfiguration( configKey, key.data(),
			sysEnv.GetVariable( key.data() ), klib::MutSourceInfo( os.GetVersionInfo().productType.data(), 0, __FUNCTION__ ) );
	}

	RemapConfigurationKey( "App", appConfigKey );
	RemapConfigurationKey( "System", configKey );
}

void TryRunApplication()
{
	try
	{
		RunApplication();
	}
	catch ( ... )
	{
		const auto report = krakoa::debug::UnwrapNestedExceptions();
		KRK_ERR( "[CRASH] Exception(s): \n" + report );
		auto& errorHandler = krakoa::os::iOperatingSystem::Reference().GetErrorHandler();
		errorHandler.EmergencyExit();
	}
}

void RunApplication()
{
	KRK_PROFILE_SESSION_BEGIN( "Start Up", "KRK_PROFILER-StartUp" );

	auto& operatingSystem = ConnectOS();

	InitializeVirtualFileSystem();
	InitializeGlobalConfig( operatingSystem );
	ConfigureLogging();

	krakoa::CreateApplication();

	auto& pApp = krakoa::Application::Reference();
	pApp.Initialize();

	KRK_PROFILE_SESSION_END();

	KRK_PROFILE_SESSION_BEGIN( "RunTime", "KRK_PROFILER-Runtime" );
	pApp.Run();
	KRK_PROFILE_SESSION_END();

	KRK_PROFILE_SESSION_BEGIN( "ShutDown", "KRK_PROFILER-ShutDown" );
	pApp.ShutDown();
	KRK_PROFILE_SESSION_END();
#endif
}

inline krakoa::os::iOperatingSystem& ConnectOS()
{
	krakoa::os::CreateOperatingSystemInterface();

	krakoa::os::iOperatingSystem::Reference().GetErrorHandler().SetEmergencyExitFunc( []()
	{
		if ( krakoa::Application::IsActive() )
			krakoa::Application::Pointer()->ShutDown();
	} );

	return krakoa::os::iOperatingSystem::Reference();
}

inline void ConfigureLogging()
{
	const auto logLevelStr = krakoa::configurations::GetConfiguration<std::string_view>( "Logging", "Level" );
	const auto maxBytes = krakoa::configurations::GetConfiguration<size_t>( "Logging", "MaxBytes" );
	const auto maxDays = krakoa::configurations::GetConfiguration<size_t>( "Logging", "MaxDays" );
	const auto minLvl = klib::LogLevel::FromString( klib::ToUpper( logLevelStr ) );

	krakoa::EngineLogger::SetMinimumLogLevelUsingConfig( minLvl );
	krakoa::EngineLogger::RemoveIfTooOldFile( maxDays );
	krakoa::EngineLogger::RemoveIfTooLarge( maxBytes );
}
