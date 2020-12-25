#pragma once

#ifdef KRAKOA_TEST
#	include "Tests/TestDriver.hpp"
#else
#	include <memory>
#	include "Instrumentor.hpp"
#	include "Core/Application.hpp"
extern void krakoa::CreateApplication();
#endif

#include <Utility/Calendar/kUseCalendarSourceInfo.hpp>
#include "Memory/Memory Structures/HeapFactory.hpp"


inline void Launch();

int main(int argc, char** argv)
{
	klib::kLocale::SetLocale("");
	memory::HeapFactory::Initialize();
	Launch();
	memory::HeapFactory::ReportMemoryLeaks();
	memory::HeapFactory::ShutDown();
	
	return EXIT_SUCCESS;
}

inline void Launch()
{
	klib::kCalendar::UsePlatformCalendarInfoSource();
	
#ifdef KRAKOA_TEST
	krakoa::tests::TestDriver::Initialize();
	krakoa::tests::TestDriver::RunAll();
	krakoa::tests::TestDriver::ShutDown();
#else
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
