#pragma once

#include "EngineCore.hpp"

namespace krakoa
{
	class KRAKOA_API FPSCounter
	{
	public:
		FPSCounter(const double updateRate);
		virtual ~FPSCounter();

		double GetFPS(const double deltaTime) ;

	private:
		double CalculateFPS(const double deltaTime) ;

	private:
		const double updateRate;
		double frameCounter;
		double dt;
	};
}
