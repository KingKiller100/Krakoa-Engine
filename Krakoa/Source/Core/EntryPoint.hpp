#pragma once

#ifdef KRAKOA_TEST
#	include "Tests/TestDriver.hpp"
#else
#	include "Application.hpp"
#	include "PointerTypes.hpp"

#	include "../Debug/Instrumentor.hpp"
#	include "../Config/GlobalConfig.hpp"

#	include "../FileSystem/VirtualFileExplorer.hpp"

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
	KRK_PROFILE_SESSION_BEGIN("Start Up", "KRK_PROFILER-StartUp");
	
	KRK_INIT_LOGS("");
	
	const std::filesystem::path cwd = klib::GetCurrentWorkingDirectory() + "..\\";

	filesystem::VirtualFileExplorer::Initialize(cwd);
	filesystem::VirtualFileExplorer::Mount("Krakoa\\Config", "Config");
	filesystem::VirtualFileExplorer::Mount("Keditor\\Assets", "Assets");
	
	configurations::GlobalConfig::Create(cwd);
	auto globalConfig = Solo_Ptr<configurations::GlobalConfig>(configurations::GlobalConfig::Pointer());

	EngineLogger::SetMinimumLogLevelUsingConfig();
	EngineLogger::RemoveOldFileUsingConfig();

	filesystem::VirtualFileExplorer::Mount("Keditor\\Assets\\Fonts", "Fonts");
	filesystem::VirtualFileExplorer::Mount("Keditor\\Assets\\Shaders", "Shaders");
	filesystem::VirtualFileExplorer::Mount("Keditor\\Assets\\Textures", "Textures");

	const auto dirs = filesystem::VirtualFileExplorer::GetDirectories("Fonts");
	const auto files = filesystem::VirtualFileExplorer::GetFiles("Fonts", ".ttf", filesystem::RECURSIVE);
	
	CreateApplication();
	auto pApp = Solo_Ptr<Application>(Application::Pointer());
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
