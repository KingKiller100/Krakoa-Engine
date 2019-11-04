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
	using namespace kTest;
	Print_Test();
	Clock_Test();
	FileSystem_Test();
	Debugger_Test();
	Calendar_Test();
	Logger_Test();
	Math_Vector2_Test();
	Math_Vector3_Test();
	Math_Vector4_Test();
	String_View_Test();
}
