#include <Precompile.hpp>
#include <Core/Application.hpp>

#include <Core/Logging/CoreLogger.hpp>
#include <Rendering/LayerBase.hpp>

#include <Utility/Timer/kTimer.hpp>

#include "Input/InputManager.hpp"

#include <Patterns/SimpleSingletonImpl.hpp> // Required for singleton base class functionality to work

namespace krakoa
{
	using namespace klib;
	kTime::HighAccuracyTimer systemTimer("Krakoa Engine Timer");

	Application::Application(Token&)
		: isRunning(false),
		fpsCounter(60)
	{
		KRK_INIT_LOGS();

		KRK_FATAL(!instance, "Instance of the application already exists!");

		pWindow = std::unique_ptr<iWindow>(iWindow::Create());
		pWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
		= default;

#include <Maths/Vectors/VectorN.hpp>
	void Application::Initialize()
	{
		isRunning = true;
		KRK_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");
		input::InputManager::Initialize();
	}

	void Application::OnEvent(events::Event& e)
	{
		events::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<events::WindowClosedEvent>([this](events::WindowClosedEvent& e) -> bool
			{
				return OnWindowClosed(e);
			});

		for (auto iter = layerStack.end(); iter != layerStack.begin();)
		{
			--iter;
			(*iter)->OnEvent(e);
			if (e.isHandled())
				break;
		}
	}

	bool Application::OnWindowClosed(events::WindowClosedEvent& e)
	{
		Shutdown();
		return true;
	}

	void Application::PushLayer(LayerBase* layer)
	{
		layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(LayerBase* overlay)
	{
		layerStack.PushOverlay(overlay);
	}

	void Application::Run()
	{
		const auto deltaTime = systemTimer.GetDeltaTime<kTime::Millis>();
		const auto fps = fpsCounter.GetFPS(deltaTime);

		layerStack.OnUpdate();
		pWindow->OnUpdate();
	}

	constexpr bool Application::IsRunning() const
	{
		return isRunning;
	}

	iWindow& Application::GetWindow() const
	{
		return *pWindow;
	}
}
