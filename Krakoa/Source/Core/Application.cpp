#include "Precompile.hpp"
#include "Application.hpp"

#include "../Logging/EngineLogger.hpp"

#include "../Debug/Debug.hpp"
#include "../Debug/Instrumentor.hpp"

#include "../Input/KeyCode.hpp"
#include "../Input/InputManager.hpp"

#include "../Layers/FPS/FPSLayer.hpp"
#include "../Layers/Logging/LoggerLayer.hpp"
#include "../Layers/Statistics/Renderer2D/StatisticLayer.hpp"

#include "../FileSystem/AssetManager.hpp"

#include "../Graphics/Renderer.hpp"
#include "../Graphics/2D/Renderer2D.hpp"

#include "../Scene/SceneManager.hpp"

#include <chrono>

namespace krakoa
{
	Application::Application(Token&, const std::string_view& appName)
		: isRunning(true)
		, timeStep(/*120*/)
		, isMinimized(false)
	{
		KRK_PROFILE_FUNCTION();

		KRK_ASSERT(!instance, "Instance of application " + std::string(appName) + " already exists!");

		//timeStep.SetSpeedMultiplier(5);

		// Initialize Window
		pWindow = std::unique_ptr<iWindow>(iWindow::Create(WindowProperties(appName)));
		pWindow->SetEventCallback(KRK_BIND_FUNC(Application::OnEvent));
	}

	Application::~Application()
		= default;

	void Application::Initialize()
	{
		KRK_PROFILE_FUNCTION();

		PushInternalLayers();

		// Initialize InputManager
		input::InputManager::Initialize();
		AddManager(input::InputManager::Pointer());

		RegisterManager<filesystem::AssetManager>();

		gfx::Renderer::Initialize();

		AddManager(new scene::SceneManager());

		frameBuffer.reset(gfx::iFrameBuffer::Create({ 1024, 640, 1, false }));

		auto& assetMan = GetManager<filesystem::AssetManager>();
		assetMan.Initialize();
		assetMan.GetFontLibrary().MakeDefault("OpenSans", 18.f);
	}
	
	void Application::PushInternalLayers()
	{
		// Initialize Layer
		pImGuiLayer = new ImGuiLayer();
		PushOverlay(pImGuiLayer);

		PushOverlay(new LoggerLayer());
		PushOverlay(new FPSLayer());
		PushOverlay(new Renderer2DStatistics());
	}

	void Application::OnEvent(events::Event& e)
	{
		KRK_PROFILE_FUNCTION();
		events::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<events::WindowClosedEvent>(KRK_BIND_FUNC(Application::OnWindowClosed));
		dispatcher.Dispatch<events::WindowResizeEvent>(KRK_BIND_FUNC(Application::OnWindowResize));

		layerStack.OnEvent(e);
	}

	bool Application::OnWindowClosed(events::WindowClosedEvent& e)
	{
		KRK_PROFILE_FUNCTION();
		isRunning = false;
		return true;
	}

	bool Application::OnWindowResize(events::WindowResizeEvent& e) noexcept
	{
		KRK_PROFILE_FUNCTION();
		isMinimized = e.GetDimensions().MagnitudeSQ() == 0.f;
		const auto width = CAST(int, e.GetWidth());
		const auto height = CAST(int, e.GetHeight());
		gfx::Renderer::OnWindowResize(0, 0, width, height);
		KRK_DBG(util::Fmt("Resize window event: ({0}, {1})", width, height));
		return false;
	}
	
	void Application::Run() const
	{
		do {
			const auto deltaTime = timeStep.GetStep();
			auto& sceneManager = GetManager<scene::SceneManager>();

			if (input::InputManager::IsKeyPressed(input::KEY_V))
				pImGuiLayer->ToggleVisibility();

			if (!isMinimized)
			{
				layerStack.OnUpdate(deltaTime);
			}

			frameBuffer->Bind();
			gfx::Renderer::Update();
			sceneManager.OnUpdate(deltaTime);
			frameBuffer->Unbind();

			pImGuiLayer->BeginDraw();
			layerStack.OnRender();
			pImGuiLayer->EndDraw();

			pWindow->OnUpdate();
		} while (IsRunning());
	}
	
	void Application::ShutDown()
	{
		KRK_BANNER("Closing App", "Shut Down", "*", "*", 10);
		gfx::Renderer::ShutDown();

		for (auto* manager : managers)
		{
			if (manager)
			{
				delete manager;
				manager = nullptr;
			}
		}

		const auto appTimeSpan = timeStep.GetLifeTimeTimeSpan();

		KRK_NRM(klib::ToString("Total Runtime: {0}h {1}m {2}s",
			appTimeSpan.hours.count()
			, appTimeSpan.minutes.count()
			, appTimeSpan.seconds.count()
		));
		
		KRK_LOG_END();
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

	void Application::Close() noexcept
	{
		isRunning = false;
	}

	bool Application::IsRunning() const
	{
		return isRunning;
	}

	iWindow& Application::GetWindow() const
	{
		return *pWindow;
	}

	gfx::iFrameBuffer& Application::GetFrameBuffer() const
	{
		return *frameBuffer;
	}

	ImGuiLayer& Application::GetImGuiLayer() const
	{
		return *pImGuiLayer;
	}

	Application& GetApp()
	{
		return Application::Reference();
	}

	iWindow& GetWindow()
	{
		return GetApp().GetWindow();
	}
}
