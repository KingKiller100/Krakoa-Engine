#include <Krakoa.hpp>
#include <Core/EntryPoint.hpp>

#include "Layers/Renderer2DLayer.hpp"


class Hooper2Game : public krakoa::Application
{
public:
	Hooper2Game(Token&& t)
		: Application(t)
	{
		INIT_LOGS("Hooper2");
		TOGGLE_SUBSYSTEM_LOGGING();
		SET_LOG_MIN(LOG_LVL_DBUG);
	}

	~Hooper2Game()
	{
		Hooper2Game::Shutdown();
	}

	void Initialize() override
	{
		Application::Initialize();

		PushLayer(new Renderer2DLayer());
	}

	void Shutdown() override
	{
		isRunning = false;
	}
};

void krakoa::CreateApplication()
{
	Application::Create<Hooper2Game>();
}