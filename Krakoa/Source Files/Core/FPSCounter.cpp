#include <Precompile.hpp>
#include <Core/FPSCounter.hpp>

namespace krakoa
{
	FPSCounter::FPSCounter(const double updateRate)
		: updateRate(updateRate),
		frameCounter(0),
		dt(0)
	{	}

	FPSCounter::~FPSCounter()
	{}

	double FPSCounter::GetFPS(const double deltaTime) 
	{
		const auto fps = CalculateFPS(deltaTime);
		return fps;
	}

	double FPSCounter::CalculateFPS(const double deltaTime) 
	{
		auto fps = 0.0;
		frameCounter++;
		dt += deltaTime;
		const auto rate = 1.0 / updateRate;
		if (dt > rate)
		{
			fps = frameCounter / dt;
			frameCounter = 0;
			dt = 0;
			dt -= rate;
		}
		return fps;
	}

}

