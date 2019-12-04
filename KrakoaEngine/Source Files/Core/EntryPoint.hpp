#pragma once

#include <Core/Application.hpp>

#include <Testing/TesterManager.hpp>
#include <Utility/Logging/kLogging.hpp>

#include <memory>

extern krakoa::Application* krakoa::CreateApplication();

inline void RunTestsOnkLibrary()
{
	auto& testManager = kTest::TesterManager::Reference();
	testManager.Initialize();
	testManager.InitializeMathsTests();
	testManager.InitializeUtilityTests();
	testManager.RunAll();
};

int main(int argv, char** argc)
{
	RunTestsOnkLibrary();

	INIT_LOGS();
	LOG_BANNER("WELCOME TO KRAKOA", "ENTRY");
		
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
