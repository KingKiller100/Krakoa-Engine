#pragma once

#include "Application.hpp"

#include <memory>

extern void krakoa::CreateApplication();

bool RunTestsOnkLibrary();

int main(int argc, char** argv)
{
	if (!RunTestsOnkLibrary())
	{
		krakoa::CreateApplication();
		auto pApp = std::unique_ptr<krakoa::Application>(krakoa::Application::Pointer());

		pApp->Initialize();

		do {
			pApp->Run();
		} while (pApp->IsRunning());

		pApp->Shutdown();
	}
	return EXIT_SUCCESS;
}



#ifdef KRAKOA_TEST
#	include <Core/TestDriver.hpp>
bool RunTestsOnkLibrary()
{
	auto testDriver = tests::TestDriver();
	testDriver.Initialize();
	testDriver.RunTests();
	testDriver.ShutDown();
	return true;
};
#else
bool RunTestsOnkLibrary() { return false; }
#endif // KRAKOA_TEST