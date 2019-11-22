#pragma once
#include "Application.h"

#include "../Beta Test/TesterManager.h"

#include <memory>

extern krakoa::Application* krakoa::CreateApplication();

void RunTestsOnkLibrary();

int main(int argv, char** argc)
{
	RunTestsOnkLibrary();
		
	const auto deleter = [](krakoa::Application* game) -> void
	{
		game->Shutdown();
		delete game;
	};

	auto app = std::unique_ptr<krakoa::Application, decltype(deleter)>(krakoa::CreateApplication(), deleter);

	app->Initialize();

	while (app->IsRunning())
	{
		app->Run();
		app->Shutdown(); // placed for now so that the application doesn't run endlessly
	}
	 
	return 0;
}


inline void RunTestsOnkLibrary()
{
	auto& testManager = kTest::TesterManager::Reference();
	testManager.Initialize();
	testManager.InitializeMathsTests();
	testManager.InitializeUtilityTests();
	testManager.RunAll();
}
