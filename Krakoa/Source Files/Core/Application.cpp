#include <Precompile.hpp>
#include <Core/Application.hpp>

#include <Core/Logger.hpp>
#include <Events System/ApplicationEvent.hpp>

#include <Utility/Timer/kTimer.hpp>

namespace krakoa
{
	using namespace klib;
	kTime::HighAccuracyTimer systemTimer("Krakoa Engine Timer");

	Application::Application()
	: isRunning(false)
	{
		KRK_INIT_LOGS();
		KRK_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");
	
		window = std::unique_ptr<iWindow>(iWindow::Create());
	}

	Application::~Application()
		= default;

	void Application::Initialize()
	{
		isRunning = true;
	}

	void Application::Run()
	{
		const auto deltaTime = systemTimer.GetDeltaTime<kTime::Millis>();
		std::cout << "dt: " << deltaTime << "\n";
		//KRK_NORM(kFormat::ToString("%02.02f fps", deltaTime));
		window->OnUpdate();
	}

	constexpr bool Application::IsRunning() const
	{
		return isRunning;
	}
}
