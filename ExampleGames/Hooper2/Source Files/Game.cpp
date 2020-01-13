#include <Krakoa.hpp>
#include <kLib.hpp>

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
		 running = false;
	}
};

krakoa::Application* krakoa::CreateApplication()
{
	return new Hooper2Game();
}