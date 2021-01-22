#pragma once

#ifdef KRAKOA_TEST
#	include "Tests/TestDriver.hpp"
#else
#	include "Application.hpp"
#	include "PointerTypes.hpp"

#	include "../Debug/Instrumentor.hpp"
#	include "../Config/GlobalConfig.hpp"

#	include <memory>
#	include <filesystem>

extern void krakoa::CreateApplication();
#endif

#include "../Memory/MemoryStructures/HeapFactory.hpp"

#include <Utility/Debug/kDebugger.hpp>
#include <Utility/Calendar/kUseCalendarSourceInfo.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

inline void Launch();

int main(int argc, char** argv)
{
	klib::kLocale::SetLocale("");
	klib::kCalendar::UsePlatformCalendarInfoSource();

	memory::HeapFactory::Initialize();
	Launch();
	memory::HeapFactory::ReportMemoryLeaks();
	memory::HeapFactory::ShutDown();

	return EXIT_SUCCESS;
}

inline void Launch()
{
	using namespace krakoa;

	klib::kDebug::IsDebuggerAttached("DebugPlease");

#ifdef KRAKOA_TEST
	krakoa::tests::TestDriver::Initialize();
	krakoa::tests::TestDriver::RunAll();
	krakoa::tests::TestDriver::ShutDown();
#else

	const std::filesystem::path cwd = klib::GetCurrentWorkingDirectory() + "..\\Krakoa\\";
	configuration::GlobalConfig::Create(cwd);
	auto globalConfig = krakoa::Solo_Ptr<configuration::GlobalConfig>(configuration::GlobalConfig::Pointer());

	KRK_INIT_LOGS("");

	KRK_PROFILE_SESSION_BEGIN("Start Up", "KRK_PROFILER-StartUp");
	krakoa::CreateApplication();
	auto pApp = std::unique_ptr<krakoa::Application>(krakoa::Application::Pointer());
	pApp->Initialize();
	KRK_PROFILE_SESSION_END();

	KRK_PROFILE_SESSION_BEGIN("RunTime", "KRK_PROFILER-Runtime");
	do {
		pApp->Run();
	} while (pApp->IsRunning());
	KRK_PROFILE_SESSION_END();

	KRK_PROFILE_SESSION_BEGIN("ShutDown", "KRK_PROFILER-ShutDown");
	pApp->ShutDown();
	KRK_PROFILE_SESSION_END();
#endif
}
