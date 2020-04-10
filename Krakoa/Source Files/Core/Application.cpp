#include "Precompile.hpp"
#include "Application.hpp"

#include "Logging/CoreLogger.hpp"

#include "../Input/InputManager.hpp"

#include <Utility/Timer/kTimer.hpp>

namespace krakoa
{
	using namespace klib;
	kTime::HighAccuracyTimerf systemTimer("Krakoa Engine Timer");

	Application::Application(Token&)
		: isRunning(false),
		fpsCounter(60)
	{
		KRK_INIT_LOGS();
		KRK_FATAL(!instance, "Instance of the application already exists!");
		KRK_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");

		// Initialize Window
		pWindow = std::unique_ptr<iWindow>(iWindow::Create());
		pWindow->SetEventCallback(KRK_BIND1(Application::OnEvent));
	}

	Application::~Application()
		= default;

	void Application::Initialize()
	{

		isRunning = true;

		// Initialize Layer
		pImGuiLayer = new ImGuiLayer();
		PushOverlay(pImGuiLayer);

		// Initialize InputManager
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
		const auto deltaTime = systemTimer.GetDeltaTime<kTime::Secs>();
		const auto fps = fpsCounter.GetFPS(deltaTime);

		layerStack.OnUpdate(deltaTime);

		pImGuiLayer->BeginDraw();
		layerStack.OnRender();
		pImGuiLayer->EndDraw();

		pWindow->OnUpdate();
	}

	bool Application::IsRunning() const
	{
		return isRunning;
	}

	iWindow& Application::GetWindow() const
	{
		return *pWindow;
	}
}
