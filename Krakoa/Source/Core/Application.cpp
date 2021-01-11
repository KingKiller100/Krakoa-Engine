#include "Precompile.hpp"
#include "Application.hpp"

#include "../Logging/CoreLogger.hpp"

#include "../Debug/Instrumentor.hpp"

#include "../Input/KeyCode.hpp"
#include "../Input/InputManager.hpp"

#include "../Layers/FPS/FPSLayer.hpp"
#include "../Layers/Statistics/Renderer2D/StatisticLayer.hpp"

#include "../Debug/Debug.hpp"
#include "../Graphics/Renderer.hpp"
#include "../Graphics/2D/Renderer2D.hpp"
#include "../Graphics/ShaderLibrary.hpp"

#include <Utility/Logging/kLogging.hpp>

namespace krakoa
{
	Application::Application(Token&, const std::string_view& appName)
		: isRunning(true),
		timeStep(/*120*/),
		isMinimized(false)
	{
		KRK_PROFILE_FUNCTION();

		KRK_INIT_LOGS("WELCOME TO THE KRAKOA ENGINE");
		KRK_SET_LOG_MIN(KRK_LOG_LVL_NRM);
		KRK_ASSERT(!instance, "Instance of the application already exists!");
		//timeStep.SetSpeedMultiplier(5);
		// Initialize Window
		pWindow = std::unique_ptr<iWindow>(iWindow::Create(WindowProperties(appName)));
		pWindow->SetEventCallback(KRK_BIND1(Application::OnEvent));
	}

	Application::~Application()
		= default;

	void Application::ShutDown()
	{
		KRK_BANNER("Shut Down", "Closing App", "*", "*", 10);
		graphics::Renderer::ShutDown();

		auto* inputPtr = input::InputManager::Pointer();
		if (inputPtr)
		{
			delete inputPtr;
			inputPtr = nullptr;
		}

		KRK_LOG_END();
	}

	void Application::Initialize()
	{
		KRK_PROFILE_FUNCTION();

		// Initialize Layer
		pImGuiLayer = new ImGuiLayer();
		PushOverlay(pImGuiLayer);

		PushOverlay(new FPSLayer());
		PushOverlay(new Renderer2DStatistics());

		// Initialize InputManager
		input::InputManager::Initialize();

		// Initialize Graphics Stuff
		graphics::ShaderLibrary::Create();
		graphics::Renderer::Initialize(graphics::ShaderLibrary::Reference());

		// Initialize Entity Manager
		EntityComponentSystem::Create();
		entityManager.reset(EntityComponentSystem::Pointer());


		graphics::FrameBufferSpecification fbSpec;
		fbSpec.width = 1024;
		fbSpec.height = 640;
		frameBuffer.reset(graphics::iFrameBuffer::Create(fbSpec));
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

	bool Application::OnWindowResize(events::WindowResizeEvent& e) noexcept
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
		const auto deltaTime = timeStep.GetStep();

		if (input::InputManager::IsKeyPressed(input::KEY_V))
			pImGuiLayer->ToggleVisibility();

		// Update
		entityManager->Update(deltaTime);

		if (!isMinimized)
		{
			layerStack.OnUpdate(deltaTime);
		}

		frameBuffer->Bind();

		graphics::Renderer::Update();
		// Draw
		entityManager->Draw();

		frameBuffer->Unbind();

		pImGuiLayer->BeginDraw();
		layerStack.OnRender();
		pImGuiLayer->EndDraw();

		pWindow->OnUpdate();
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

	Multi_Ptr<graphics::iFrameBuffer>& Application::GetFrameBuffer()
	{
		return frameBuffer;
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
