#include <Krakoa.hpp>

class Hooper2Game : public krakoa::Application
{
public:
	Hooper2Game()
		: Application()
	{}
		
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