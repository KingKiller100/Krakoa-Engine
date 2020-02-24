#pragma once

#include <Core/Application.hpp>

#include <memory>

extern krakoa::Application* krakoa::CreateApplication();

void RunTestsOnkLibrary();

int main(int argc, char** argv)
{
	RunTestsOnkLibrary();

	auto app = std::unique_ptr<krakoa::Application>(krakoa::CreateApplication());

	app->Initialize();
	
	unsigned count = 0;
	do {
		app->Run();
		
		count++;

		if (count > 10000)
			app->Shutdown(); // placed for now so that the application doesn't run endlessly
	} while (app->IsRunning());
	 
	return 0;
}




#ifdef KRAKOA_TEST
#include <Testing/TesterManager.hpp>
	inline void RunTestsOnkLibrary()
	{
		auto& testManager = kTest::TesterManager::Reference();
		testManager.Initialize();
		testManager.InitializeMathsTests();
		testManager.InitializeUtilityTests();
		testManager.RunAll();
		testManager.Shutdown();
	};
#else
	inline void RunTestsOnkLibrary() {	}
#endif // KRAKOA_TEST
