#pragma once

#include <Utility/Logging/kLogging.hpp>

#include <Core/Application.hpp>

#include <memory>

extern krakoa::Application* krakoa::CreateApplication();

void RunTestsOnkLibrary();

int main(int argc, char** argv)
{
	RunTestsOnkLibrary();

	INIT_LOGS();
	LOG_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");

	auto app = std::unique_ptr<krakoa::Application>(krakoa::CreateApplication());

	app->Initialize();

	while (app->IsRunning())
	{
		app->Run();
		app->Shutdown(); // placed for now so that the application doesn't run endlessly
	}
	 
	return 0;
}




#ifdef TEST_ALLOWED
#include <Testing/TesterManager.hpp>
inline void RunTestsOnkLibrary()
{
	auto& testManager = kTest::TesterManager::Reference();
	testManager.Initialize();
	testManager.InitializeMathsTests();
	testManager.InitializeUtilityTests();
	testManager.RunAll();
};
#else
inline void RunTestsOnkLibrary()
{	}
#endif // TEST_ALLOWEDG
