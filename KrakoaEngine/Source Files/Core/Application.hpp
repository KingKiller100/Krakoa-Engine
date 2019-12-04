#pragma once

#include <Core/EngineCore.hpp>
#include <Timer/kTimer.hpp>

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
		util::kTime::SystemTimer systemTimer;
	};

	Application* CreateApplication();
}