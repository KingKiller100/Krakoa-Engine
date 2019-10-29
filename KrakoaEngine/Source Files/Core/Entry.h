#pragma once
#include "Beta Test/Tester.h"
#include <memory>

using namespace krakoa;

void Tests();

#ifdef KRAKOA_OS_WINDOWS

extern krakoa::Application* krakoa::CreateApplication();

int main(int argv, char** argc)
{
	Tests();
	
	const auto deletor = [](krakoa::Application* game) -> void
	{
		game->Shutdown();
		delete game;
	};

	auto app = std::unique_ptr<krakoa::Application, decltype(deletor)>(krakoa::CreateApplication(), deletor);

	app->Initialize();

	while (app->IsRunning())
	{
		app->Run();
		app->Shutdown(); // placed for now so that the application doesn't run endlessly
	}

	return 0;
}

#endif

inline void Tests()
{
	kTest::Print_Test();
	kTest::Clock_Test();
	kTest::FileSystem_Test();
	kTest::Debugger_Test();
	kTest::Calendar_Test();
	kTest::Logger_Test();	
}
