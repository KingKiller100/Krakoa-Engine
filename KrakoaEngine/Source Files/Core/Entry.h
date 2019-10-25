#pragma once
#include <memory>

#ifdef KRAKOA_OS_WINDOWS
#include "Utility/Format To String/kFormatToString.h"
#include "Utility/Date And Time/kTime.h"

extern krakoa::Application* krakoa::CreateApplication();

int main(int argv, char** argc)
{
	std::string str = "purple";
	
	krakoa::kFormatToString::FormatToString("%s rain %s", str.c_str(), krakoa::kTime::Clock<>::GetCurrentTime().c_str());
	
	krakoa::kTest::PrintTest();
	
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
