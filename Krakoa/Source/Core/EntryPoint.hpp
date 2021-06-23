#pragma once

#ifdef KRAKOA_TEST
#	include "Tests/TestDriver.hpp"
#else
#	include "Application.hpp"
#	include "PointerTypes.hpp"

#	include "../Debug/Instrumentor.hpp"
#	include "../Debug/ExceptionHandler.hpp"
#	include "../Config/GlobalConfig.hpp"

#	include "../FileSystem/VirtualFileExplorer.hpp"

#	include <Utility/Debug/kDebugger.hpp>

#	include <memory>
#	include <filesystem>

extern void krakoa::CreateApplication();
#endif

#include "../Memory/MemoryStructures/HeapFactory.hpp"

#include <Utility/Localization/kLocale.hpp>
#include <Utility/Calendar/kUseCalendarSourceInfo.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

inline void Launch(int argc, char** argv);
inline void InitializeVirtualFileSystem();
inline void InitializeGlobalConfig(int argc, char** argv);
inline void TryRunApplication();
inline void RunApplication();

using namespace krakoa;

int main(int argc, char** argv)
{
	klib::kLocale::SetLocale("");
	klib::kCalendar::UsePlatformCalendarInfoSource();

	memory::HeapFactory::Initialize();
	Launch(argc, argv);
	memory::HeapFactory::ReportMemoryLeaks();
	memory::HeapFactory::ShutDown();

	return EXIT_SUCCESS;
}

inline void Launch(int argc, char** argv)
{

#ifdef KRAKOA_TEST
	krakoa::tests::TestDriver::Initialize();
	krakoa::tests::TestDriver::RunAll();
	krakoa::tests::TestDriver::ShutDown();
#else
	KRK_PROFILE_SESSION_BEGIN("Start Up", "KRK_PROFILER-StartUp");

	KRK_INIT_LOGS("");

	InitializeVirtualFileSystem();
	InitializeGlobalConfig(argc, argv);

	EngineLogger::SetMinimumLogLevelUsingConfig();
	EngineLogger::RemoveIfTooOldFile();

	os::CreateOperatingSystemInfo();
	
	CreateApplication();

	auto& operatingSystem = os::iOperatingSystem::Reference();

	operatingSystem.Initialize();
	operatingSystem.GetErrorHandler().SetEmergencyExitFunc([]()
	{
		Application::Pointer()->ShutDown();
	});

	if (klib::ScanForDebugger(std::chrono::milliseconds(500)))
	{
		RunApplication();
	}
	else
	{
		TryRunApplication();
	}
	Application::Destroy();
	configurations::GlobalConfig::Destroy();
	os::DestroyOperatingSystemInfo();
}

inline void InitializeVirtualFileSystem()
{
	const std::filesystem::path cwd = klib::GetCurrentWorkingDirectory() + "..\\";
	filesystem::VirtualFileExplorer::Initialize(cwd);
	filesystem::VirtualFileExplorer::Mount("Krakoa\\Config", "Config");
	filesystem::VirtualFileExplorer::Mount("Keditor\\Assets", "Assets");
}

inline void InitializeGlobalConfig(int argc, char** argv)
{
	configurations::GlobalConfig::Create();
	auto& globalConfig = configurations::GlobalConfig::Reference();

	globalConfig.Set("Application", "CmdArguments", argc, MUT_SRC_INFO());
	for (auto i = 0; i < argc; ++i)
	{
		globalConfig.Set("Application", util::Fmt("CmdArgument{0}", i), argv[i], MUT_SRC_INFO());
	}
	globalConfig.Set("Application", "Configuration", klib::GetRuntimeConfigurationStr(), MUT_SRC_INFO());
}

inline void TryRunApplication()
{
	try
	{
		RunApplication();
	}
	catch (...)
	{
		KRK_LOG("CRASH", "Exception(s): " + debug::UnwrapNestedExceptions());
		auto& errorHandler = os::iOperatingSystem::Reference().GetErrorHandler();
		errorHandler.CheckForNewError();
		const auto errCode = errorHandler.GetCode();
		const auto errText = errorHandler.GetText();
		KRK_LOG("CRASH", util::Fmt("[System] 0x{0:4h}: {1}", errCode, errText));
	}
}

inline void RunApplication()
{

	auto& pApp = Application::Reference();
	pApp.Initialize();
	KRK_PROFILE_SESSION_END();

	KRK_PROFILE_SESSION_BEGIN("RunTime", "KRK_PROFILER-Runtime");
	pApp.Run();
	KRK_PROFILE_SESSION_END();

	KRK_PROFILE_SESSION_BEGIN("ShutDown", "KRK_PROFILER-ShutDown");
	pApp.ShutDown();
	KRK_PROFILE_SESSION_END();
#endif
}

