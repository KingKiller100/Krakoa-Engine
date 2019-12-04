#pragma once

#include <Core/EngineCore.hpp>

namespace krakoa
{
	class KRAKOA_API Application
	{
	public:
		Application();
		virtual ~Application();

		constexpr void Initialize();
		void Run();
		constexpr  bool IsRunning() const;
		virtual void Shutdown() = 0;

	protected:
		bool running;
	};

	Application* CreateApplication();
}