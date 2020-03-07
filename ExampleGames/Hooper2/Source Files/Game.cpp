#include <Krakoa.hpp>

class DemoLayer  : public krakoa::LayerBase
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
		//DBUG(__FUNCSIG__);
	}
};

class Hooper2Game : public krakoa::Application
{
public:
	Hooper2Game()
		: Application()
	{
		INIT_LOGS();
		PushLayer(new DemoLayer());
	}
		
	~Hooper2Game()
	{
		Shutdown();
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