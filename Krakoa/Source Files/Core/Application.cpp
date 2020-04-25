#include "Precompile.hpp"
#include "Application.hpp"

#include "Logging/CoreLogger.hpp"

#include "../Input/InputManager.hpp"

#include "../Layers/FPS/FPSLayer.hpp"
#include "../Layers/Profiler/ProfilerLayer.hpp"

#include "../Rendering/Renderer.hpp"
#include "../Rendering/Renderer2D.hpp"
#include "../Rendering/ShaderLibrary.hpp"

#include <Utility/Debug Helper/kDebugger.hpp>

namespace krakoa
{
	using namespace klib;

	Application::Application(Token&)
		: isRunning(true),
		timeStep(),
		isMinimized(false)
	{
		klib::kDebug::CheckRemoteDebuggerAttached("DebugPlease");

		KRK_INIT_LOGS();
		KRK_FATAL(!instance, "Instance of the application already exists!");
		KRK_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");

		// Initialize Window
		pWindow = std::unique_ptr<iWindow>(iWindow::Create());
		pWindow->SetEventCallback(KRK_BIND1(Application::OnEvent));
	}

	Application::~Application()
	{
		graphics::Renderer::ShutDown();
	}


	void Application::Initialize()
	{
		// Initialize Layer
		pImGuiLayer = new ImGuiLayer();
		pImGuiLayer->ToggleVisibility();
		PushOverlay(pImGuiLayer);

		PushOverlay(new ProfilerLayer());
		PushOverlay(new FPSLayer());

		// Initialize InputManager
		input::InputManager::Initialize();

		// Initialize Graphics Stuff
		graphics::ShaderLibrary::Create();
		graphics::Renderer::Initialize();
	}

	void Application::OnEvent(events::Event& e)
	{
		events::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<events::WindowClosedEvent>(KRK_BIND1(Application::OnWindowClosed));
		dispatcher.Dispatch<events::WindowResizeEvent>(KRK_BIND1(Application::OnWindowResize));

		layerStack.OnEvent(e);
	}

	bool Application::OnWindowClosed(events::WindowClosedEvent& e)
	{
		isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(events::WindowResizeEvent & e) noexcept
	{
		isMinimized = e.GetDimensions().MagnitudeSQ() == 0.f;
		const auto width = CAST(int, e.GetWidth());
		const auto height = CAST(int, e.GetHeight());
		graphics::Renderer::OnWindowResize(0, 0, width, height);
		return false;
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
		timeStep.Update();

		const auto deltaTime = timeStep.GetDeltaTime();

		if (!isMinimized)
		{
			layerStack.OnUpdate(deltaTime);
		}

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
