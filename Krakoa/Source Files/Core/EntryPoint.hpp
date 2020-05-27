#pragma once

#ifdef KRAKOA_TEST
#	include <Core/TestDriver.hpp>
#else
#include "Application.hpp"
#include "../Instrumentor.hpp"
#include <memory>

extern void krakoa::CreateApplication();
#endif


int main(int argc, char** argv)
{
#ifdef KRAKOA_TEST
	tests::TestDriver::Initialize();
	tests::TestDriver::RunAll();
	tests::TestDriver::ShutDown();
#else
	{
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
	}
#endif
	return EXIT_SUCCESS;
}
