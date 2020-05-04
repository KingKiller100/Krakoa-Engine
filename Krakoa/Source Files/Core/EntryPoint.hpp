#pragma once

#include "Application.hpp"

#include "../Instrumentor.hpp"

#include <memory>

extern void krakoa::CreateApplication();

bool RunTestsOnkLibrary();

int main(int argc, char** argv)
{
	if (!RunTestsOnkLibrary())
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
		pApp->Shutdown();
		KRK_PROFILE_SESSION_END();
	}
	return EXIT_SUCCESS;
}



#ifdef KRAKOA_TEST
#	include <Core/TestDriver.hpp>
bool RunTestsOnkLibrary()
{
	tests::TestDriver::Initialize();
	tests::TestDriver::RunTests();
	tests::TestDriver::ShutDown();
	return true;
};
#else
bool RunTestsOnkLibrary() { return false; }
#endif // KRAKOA_TEST