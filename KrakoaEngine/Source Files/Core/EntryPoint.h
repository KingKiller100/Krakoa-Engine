#pragma once
#include "Application.h"

#include "../Beta Test/TesterManager.h"

#include <memory>

extern krakoa::Application* krakoa::CreateApplication();

void RunTests();

int main(int argv, char** argc)
{	
	RunTests();
		
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


inline void RunTests()
{
	auto& testManager = kTest::TesterManager::Reference();
	testManager.InitializeMathsTests();
	testManager.InitializeUtilityTests();
	testManager.RunAll();
}
