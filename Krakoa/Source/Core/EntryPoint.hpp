#pragma once

#ifdef KRAKOA_TEST
#	include "Tests/TestDriver.hpp"
#else
#	include "Application.hpp"
#	include "PointerTypes.hpp"

#	include "../Debug/Instrumentor.hpp"
#	include "../Debug/ExceptionHandler.hpp"
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

int main()
{
	klib::kLocale::SetDefaultLocale();
	klib::kCalendar::UsePlatformCalendarInfoSource();

	memory::HeapFactory::Initialize();
	Launch();
	memory::HeapFactory::ReportMemoryLeaks();
	memory::HeapFactory::ShutDown();

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
	KRK_PROFILE_SESSION_BEGIN("Start Up", "KRK_PROFILER-StartUp");

	KRK_INIT_LOGS("");

	krakoa::os::CreateOperatingSystemInterface();
	auto& operatingSystem = krakoa::os::iOperatingSystem::Reference();

	InitializeVirtualFileSystem();
	InitializeGlobalConfig(operatingSystem);

	krakoa::EngineLogger::SetMinimumLogLevelUsingConfig();
	krakoa::EngineLogger::RemoveIfTooOldFile();

	krakoa::CreateApplication();
	operatingSystem.GetErrorHandler().SetEmergencyExitFunc([]()
	{
		krakoa::Application::Pointer()->ShutDown();
	});

	if (klib::ScanForDebugger(std::chrono::milliseconds(500)))
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
	krakoa::filesystem::VirtualFileExplorer::Initialize(cwd);
	krakoa::filesystem::VirtualFileExplorer::Mount("Krakoa\\Config", "Config");
	krakoa::filesystem::VirtualFileExplorer::Mount("Keditor\\Assets", "Assets");
}

void InitializeGlobalConfig( const krakoa::os::iOperatingSystem& os )
{
	using namespace krakoa::configurations;

	const auto& sysEnv = os.GetEnvironmentVariables();
	const auto envKeys = sysEnv.GetKeys();

	GlobalConfig::Create();
	
	constexpr char appConfigKey[] = "Application";
	SetConfiguration(appConfigKey, "CmdArguments", __argc, MUT_SRC_INFO());
	for (auto i = 0; i < __argc; ++i)
	{
		SetConfiguration(appConfigKey, util::Fmt("CmdArgument{0}", i), __argv[i], MUT_SRC_INFO());
	}
	SetConfiguration(appConfigKey, "Configuration", klib::GetRuntimeConfigurationStr(), MUT_SRC_INFO());

	const char configKey[] = "OS";
	for (auto&& key : envKeys)
	{
		SetConfiguration(configKey, key.data(),
			sysEnv.GetVariable(key.data()), klib::MutSourceInfo(os.GetVersionInfo().productType.data(), 0, __FUNCTION__));
	}

	RemapConfigurationKey("App", appConfigKey);
	RemapConfigurationKey("System", configKey);
}

void TryRunApplication()
{
	try
	{
		RunApplication();
	}
	catch (...)
	{
		KRK_LOG("CRASH", "Exception(s): " + krakoa::debug::UnwrapNestedExceptions());
		auto& errorHandler = krakoa::os::iOperatingSystem::Reference().GetErrorHandler();
		errorHandler.CheckForNewError();
		const auto errCode = errorHandler.GetCode();
		const auto errText = errorHandler.GetText();
		KRK_LOG("CRASH", util::Fmt("[System] 0x{0:4h}: {1}", errCode, errText));
	}
}

void RunApplication()
{
	auto& pApp = krakoa::Application::Reference();
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

