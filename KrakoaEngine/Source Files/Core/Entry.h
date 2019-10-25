#pragma once
#include <memory>

#ifdef KRAKOA_OS_WINDOWS

extern krakoa::Application* krakoa::CreateApplication();

int main(int argv, char** argc)
{
	krakoa::test::PrintTest();
	
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
