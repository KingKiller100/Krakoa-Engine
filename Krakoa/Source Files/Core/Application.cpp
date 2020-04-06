#include "Precompile.hpp"
#include "Application.hpp"

#include "Logging/CoreLogger.hpp"
#include "../Rendering/LayerBase.hpp"

#include "../Input/InputManager.hpp"

#include "../Patterns/SimpleSingletonImpl.hpp"

#include <Utility/Timer/kTimer.hpp>
#include <Maths/Matrices/PredefinedMatrices.hpp>
#include <Maths/Vectors/PredefinedVectors.hpp>

#include <glad/glad.h>

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
	}

	Application::~Application()
		= default;

	void Application::Initialize()
	{
		KRK_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");

		isRunning = true;

		pImGuiLayer = new ImGuiLayer();
		PushOverlay(pImGuiLayer);


		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		kmaths::Matrix3x3f vertices;
		vertices[0] = { -0.5f, -0.5f };
		vertices[1] = { 0.5f, -0.5f, 0.f };
		vertices[2] = { 0.f, 0.5f, 0.f };

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.GetPointerToData(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, vertices.GetRows(), GL_FLOAT, GL_FALSE, vertices.GetColumns() * sizeof(float), nullptr);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		unsigned indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

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

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
