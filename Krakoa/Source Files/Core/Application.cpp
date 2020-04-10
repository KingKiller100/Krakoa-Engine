#include "Precompile.hpp"
#include "Application.hpp"

#include "Logging/CoreLogger.hpp"

#include "../Input/InputManager.hpp"

#include "../Rendering/LayerBase.hpp"
#include "../Rendering/Renderer.hpp"
#include "../Rendering/RenderCommand.hpp"
#include "../Rendering/Rendering Resources//BufferLayout.hpp"
#include "../Platform/OpenGL/OpenGLShader.hpp"

#include <Utility/Timer/kTimer.hpp>
#include <Maths/Matrices/PredefinedMatrices.hpp>
#include <Maths/Vectors/PredefinedVectors.hpp>


namespace krakoa
{
	using namespace klib;
	kTime::HighAccuracyTimer systemTimer("Krakoa Engine Timer");

	Application::Application(Token&)
		: isRunning(false),
		fpsCounter(60),
		camera(-1.6f, 1.6f, -1.f, 1.f) // Aspect ratio from window size
	{
		KRK_INIT_LOGS();
		KRK_FATAL(!instance, "Instance of the application already exists!");

		// Initialize Window
		pWindow = std::unique_ptr<iWindow>(iWindow::Create());
		pWindow->SetEventCallback(KRK_BIND1(Application::OnEvent));
	}

	Application::~Application()
		= default;

	void Application::Initialize()
	{
		KRK_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");

		isRunning = true;

		graphics::Renderer::Create();
		graphics::Renderer::Reference().BeginScene(camera);

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
		const auto deltaTime = systemTimer.GetDeltaTime<kTime::Millis>();
		const auto fps = fpsCounter.GetFPS(deltaTime);

		layerStack.OnUpdate();

		pImGuiLayer->BeginDraw();
		layerStack.OnRender();
		pImGuiLayer->EndDraw();

		camera.SetPosition({ 0.5f, 0.5f, 0.0f });
		camera.SetRotation(45.f);

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
