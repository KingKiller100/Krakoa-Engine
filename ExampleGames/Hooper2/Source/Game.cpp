#include <Krakoa.hpp>
#include <Core/EntryPoint.hpp>

class Hooper2Game : public krakoa::Application
{
public:
	Hooper2Game(Token&& t)
		: Application(t)
	{
		INIT_LOGS("Hooper2");
		TOGGLE_CONSOLE_LOGGING();
		SET_LOG_MIN(LOG_LVL_DBG);
	}

	~Hooper2Game() override
		= default;

	void Initialize() override
	{
		Application::Initialize();
	}

	void ShutDown() override
	{
		isRunning = false;
		Application::ShutDown(); // Must be the last function call in this function
	}
};

void krakoa::CreateApplication()
{
	Application::Create<Hooper2Game>();
}