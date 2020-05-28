#include "Precompile.hpp"
#include "Application.hpp"

#include "Logging/CoreLogger.hpp"

#include "../Instrumentor.hpp"

#include "../Input/KeyCode.hpp"
#include "../Input/InputManager.hpp"

#include "../Layers/FPS/FPSLayer.hpp"
#include "../Layers/Statistics/Renderer2D/StatisticLayer.hpp"

#include "../Graphics/Renderer.hpp"
#include "../Graphics/Renderer2D.hpp"
#include "../Graphics/ShaderLibrary.hpp"

#include <Utility/Debug Helper/kDebugger.hpp>


namespace krakoa
{
	using namespace klib;

	Application::Application(Token&)
		: isRunning(true),
		timeStep(/*120*/),
		isMinimized(false)
	{
		KRK_PROFILE_FUNCTION();

		klib::kDebug::CheckRemoteDebuggerAttached("DebugPlease");

		KRK_INIT_LOGS();
		KRK_SET_LOG_MIN(KRK_LOG_LVL_DBUG);
		KRK_FATAL(!instance, "Instance of the application already exists!");
		KRK_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");

		// Initialize Window
		pWindow = std::unique_ptr<iWindow>(iWindow::Create());
		pWindow->SetEventCallback(KRK_BIND1(Application::OnEvent));
	}

	Application::~Application()
	{
		const auto size = memory::HeapFactory::GetSize();

		for (auto i = 0; i < size; ++i)
		{
			memory::HeapFactory::WalkTheHeap(i);
		}
		memory::HeapFactory::GetDefaultHeap();
		
		entityManager.reset();
		graphics::Renderer::ShutDown();
	}


	void Application::Initialize()
	{
		KRK_PROFILE_FUNCTION();

		// Initialize Layer
		pImGuiLayer = new ImGuiLayer();
		//pImGuiLayer->ToggleVisibility();
		PushOverlay(pImGuiLayer);

		PushOverlay(new FPSLayer());
		PushOverlay(new Renderer2DStatistics());

		// Initialize InputManager
		input::InputManager::Initialize();

		// Initialize Graphics Stuff
		graphics::ShaderLibrary::Create();
		graphics::Renderer::Initialize(graphics::ShaderLibrary::Reference());

		// Initialize Entity Manager
		EntityManager::Create();
		entityManager.reset(EntityManager::Pointer());
	}

	void Application::OnEvent(events::Event& e)
	{
		KRK_PROFILE_FUNCTION();
		events::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<events::WindowClosedEvent>(KRK_BIND1(Application::OnWindowClosed));
		dispatcher.Dispatch<events::WindowResizeEvent>(KRK_BIND1(Application::OnWindowResize));

		layerStack.OnEvent(e);
	}

	bool Application::OnWindowClosed(events::WindowClosedEvent& e)
	{
		KRK_PROFILE_FUNCTION();
		isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(events::WindowResizeEvent & e) noexcept
	{
		KRK_PROFILE_FUNCTION();
		isMinimized = e.GetDimensions().MagnitudeSQ() == 0.f;
		const auto width = CAST(int, e.GetWidth());
		const auto height = CAST(int, e.GetHeight());
		graphics::Renderer::OnWindowResize(0, 0, width, height);
		return false;
	}

	void Application::PushLayer(LayerBase* layer)
	{
		KRK_PROFILE_FUNCTION();
		layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(LayerBase* overlay)
	{
		KRK_PROFILE_FUNCTION();
		layerStack.PushOverlay(overlay);
	}

	void Application::PopLayer(LayerBase* layer)
	{
		KRK_PROFILE_FUNCTION();
		layerStack.PopLayer(layer);
	}

	void Application::PopOverlay(LayerBase* overlay)
	{
		KRK_PROFILE_FUNCTION();
		layerStack.PopOverlay(overlay);
	}

	void Application::Run() const
	{
		RendererClear();

		const auto deltaTime = timeStep.GetDeltaTime();

		if (input::InputManager::IsKeyPressed(KRK_KEY_I))
			pImGuiLayer->ToggleVisibility();

		// Update
		entityManager->Update(deltaTime);

		if (!isMinimized)
		{
			layerStack.OnUpdate(deltaTime);
		}

		// Draw
		entityManager->Draw();
		
		pImGuiLayer->BeginDraw();
		layerStack.OnRender();
		pImGuiLayer->EndDraw();

		pWindow->OnUpdate();
	}

	void Application::RendererClear() noexcept
	{
		KRK_PROFILE_FUNCTION();

#ifdef _DEBUG
		krakoa::graphics::Renderer::SetClearColour({ 0.85f, 0.35f, 0.f, 1.f }); // Orange background colour
#else
		krakoa::graphics::Renderer::SetClearColour({ 0.05f, 0.05f, 0.05f, 1.f }); // Black background colour
#endif // DEBUG

		krakoa::graphics::Renderer::Clear();
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
