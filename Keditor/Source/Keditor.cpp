#include <Krakoa.hpp>
#include <Core/EntryPoint.hpp>

#include "Layers/Keditor2DLayer.hpp"


class Keditor : public krakoa::Application
{
public:
	Keditor(Token&& t)
		: Application(t, "Keditor")
	{
		INIT_LOGS("Hooper2");
		TOGGLE_CONSOLE_LOGGING();
		SET_LOG_MIN(LOG_LVL_DBG);
	}

	~Keditor() override
		= default;

	void Initialize() override
	{
		Application::Initialize();

		PushLayer(new Keditor2DLayer());
	}

	void ShutDown() override
	{
		isRunning = false;
		Application::ShutDown(); // Must be the last function call in this function
	}
};

void krakoa::CreateApplication()
{
	Application::Create<Keditor>();
}