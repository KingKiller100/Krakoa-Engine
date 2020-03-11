#include <Krakoa.hpp>

class DemoLayer : public krakoa::LayerBase
{
public:
	DemoLayer()
		: LayerBase("Demo")
	{}

	// Inherited via LayerBase
	void OnAttach() override
	{
		//INFO(__FUNCSIG__);
	}
	void OnDetach() override
	{
		//INFO(__FUNCSIG__);
	}
	void OnUpdate() override
	{
		//INFO(__FUNCSIG__);
	}
	void OnEvent(krakoa::events::Event& e) override
	{
		DBUG(e.ToString());
	}
};

class Hooper2Game : public krakoa::Application
{
public:
	Hooper2Game()
		: Application(Token())
	{
		INIT_LOGS("Hooper2");
		SET_LOG_MIN(LOG_LVL_DBUG);
		PushLayer(new DemoLayer());
		PushOverlay(new krakoa::ImGuiLayer());
	}
		
	~Hooper2Game()
	{
		Hooper2Game::Shutdown();
	}
	
	void Shutdown() override
	{
		 isRunning = false;
	}
};

krakoa::Application* krakoa::CreateApplication()
{
	return new Hooper2Game();
}
