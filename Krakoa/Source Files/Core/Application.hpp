#pragma once

#include <Core/EngineCore.hpp>
#include <iWindow.hpp>

#include <memory>

namespace krakoa
{
	class KRAKOA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Initialize();
		void Run();
		constexpr  bool IsRunning() const;
		virtual void Shutdown() = 0;

	protected:
		bool isRunning;
		std::unique_ptr<iWindow> window;
	};

	Application* CreateApplication();
}