#include <Krakoa.h>

class Hooper2Game : public krakoa::Application
{
public:
	Hooper2Game()
		: Application()
	{}

	
	~Hooper2Game()
	= default;
	
	 void Shutdown() override
	{
		 running = false;
	}
};


krakoa::Application* krakoa::CreateApplication()
{
	return new Hooper2Game();
}