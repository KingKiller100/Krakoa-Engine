#pragma once

#include "EngineCore.h"

namespace krakoa
{
	class KRAKOA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Initialize();
		static void Run();
		constexpr  bool IsRunning() const;
		virtual void Shutdown() = 0;

	protected:
		bool running;
	};

	Application* CreateApplication();
}