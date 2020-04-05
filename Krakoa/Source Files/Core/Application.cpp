#include "Precompile.hpp"
#include "Application.hpp"

#include "Logging/CoreLogger.hpp"
#include "../Rendering/LayerBase.hpp"

#include "../Input/InputManager.hpp"

#include "../Patterns/SimpleSingletonImpl.hpp"

#include <Utility/Timer/kTimer.hpp>


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
		pWindow->SetEventCallback(KRK_BIND1(Application::OnEvent));

		pImGuiLayer = new ImGuiLayer();
		PushOverlay(pImGuiLayer);
	}

	Application::~Application()
		= default;

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

		pImGuiLayer->BeginDraw();
		layerStack.OnRender();
		pImGuiLayer->EndDraw();

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
