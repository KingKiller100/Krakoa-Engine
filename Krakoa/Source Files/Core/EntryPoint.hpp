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
#	include <Core/TestDriver.hpp>
	void RunTestsOnkLibrary()
	{
		auto testDriver = tests::TestDriver();
		testDriver.Initialize();
		testDriver.RunTests();
		testDriver.ShutDown();
	};
#else
	inline void RunTestsOnkLibrary() {	}
#endif // KRAKOA_TEST
