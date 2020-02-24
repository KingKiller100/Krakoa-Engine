#include <Precompile.hpp>

#include <Core/Application.hpp>
#include <Events System/ApplicationEvent.hpp>

#include <Utility/Timer/kTimer.hpp>
#include <Core/Logger.hpp>

#include <Maths/Matrix/Matrix4x4.hpp>

namespace krakoa
{
	using namespace klib;
	kTime::SystemTimer systemTimer = kTime::SystemTimer("Krakoa Engine Timer");

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

		Matrix4x4 m = Matrix4x4();
		m._m1 = Vector4f(1.f, 1);
		m._m2 = Vector4f(2.f, 1);
		m._m3 = Vector4f(3.f, 1);
		m._m4 = Vector4f(4.f, 1);
		m.Identity();
		
		Matrix4x4 m2 = Matrix4x4();
		m2._m1 = Vector4f(1.f, 1);
		m2._m2 = Vector4f(2.f, 1);
		m2._m3 = Vector4f(3.f, 1);
		m2._m4 = Vector4f(4.f, 1);

		const auto m3 = m + m2;
		const auto m4 = m * m2;

	}

	void Application::Run()
	{
		const auto deltaTime = systemTimer.GetDeltaTime<kTime::Millis>();

		window->OnUpdate();
	}

	constexpr bool Application::IsRunning() const
	{
		return isRunning;
	}
}
