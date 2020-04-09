#include "Precompile.hpp"
#include "Application.hpp"

#include "Logging/CoreLogger.hpp"

#include "../Input/InputManager.hpp"

#include "../Rendering/LayerBase.hpp"
#include "../Rendering/Renderer.hpp"
#include "../Rendering/RenderCommand.hpp"
#include "../Rendering/Graphics/BufferLayout.hpp"
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
		fpsCounter(60)
	{
		KRK_INIT_LOGS();
		KRK_FATAL(!instance, "Instance of the application already exists!");
	}

	Application::~Application()
		= default;

	void Application::Initialize()
	{
		KRK_BANNER("WELCOME TO THE KRAKOA ENGINE", "ENTRY");

		isRunning = true;

		// Initialize Window
		pWindow = std::unique_ptr<iWindow>(iWindow::Create());
		pWindow->SetEventCallback(KRK_BIND1(Application::OnEvent));

		// Initialize Layer
		pImGuiLayer = new ImGuiLayer();
		PushOverlay(pImGuiLayer);

		// Initialize InputManager
		input::InputManager::Initialize();

		graphics::Renderer::Create();

		pTriangeVA = std::unique_ptr<graphics::iVertexArray>(graphics::iVertexArray::Create());
		pSquareVA = std::unique_ptr<graphics::iVertexArray>(graphics::iVertexArray::Create());

		// Triangle creation code
		{
			// Vertices points
			kmaths::Matrix<float, 3, 7> vertices = {
				{ -0.5f, -0.5f, 0.f, 0.95f, 0.3f, 0.10f, 1.0f },
				{ 0.5f, -0.5f, 0.f, 0.25f, 0.9f, 0.85f, 1.0f },
				{ 0.0f, 0.5f, 0.0f, 0.95f, 0.3f, 0.78f, 1.0f }
			};

			// Vertex buffer
			auto pVertexBuffer = graphics::iVertexBuffer::Create(vertices.GetPointerToData(), sizeof(vertices));

			pVertexBuffer->SetLayout({
				{graphics::ShaderDataType::FLOAT3, "a_Position"},
				{graphics::ShaderDataType::FLOAT4, "a_Colour"}
				}
			);

			pTriangeVA->AddVertexBuffer(pVertexBuffer);

			// Index buffer
			uint32_t indices[3] = { 0, 1, 2 };
			pTriangeVA->SetIndexBuffer(graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);


			const std::string_view triangleVS = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Colour;

			out vec3 v_Position;
			out vec4 v_Colour;

			void main()
			{
				v_Colour = a_Colour;
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";


			const std::string_view triangleFS = R"(
			#version 330 core

			layout(location = 0) out vec4 out_color;

			in vec3 v_Position;
			in vec4 v_Colour;

			void main()
			{
				out_color = v_Colour;
			}
		)";

			pTriangleShader = std::unique_ptr<graphics::iShader>(new graphics::OpenGLShader(triangleVS, triangleFS));
		}

		// Square creation code
		{
			kmaths::Matrix4x3f squareVertices = {
				{ -0.75f, -0.75f, 0.f},
				{ 0.75f, -0.75f, 0.f },
				{ 0.75f, 0.75f, 0.0f },
				{ -0.75f, 0.75f, 0.0f }
			};

			// Vertex buffer
			auto squareVB = graphics::iVertexBuffer::Create(squareVertices.GetPointerToData(), sizeof(squareVertices));

			squareVB->SetLayout({
				{graphics::ShaderDataType::FLOAT3, "a_Position"}
				}
			);

			pSquareVA->AddVertexBuffer(squareVB);

			// Index buffer
			uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
			pSquareVA->SetIndexBuffer(graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);


			const std::string_view squareVS = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";


			const std::string_view squareFS = R"(
			#version 330 core

			layout(location = 0) out vec4 out_color;

			void main()
			{
				out_color = vec4(0.0, 0.5, 0.25, 0.50);
			}
		)";

			pSquareShader = std::unique_ptr<graphics::iShader>(new graphics::OpenGLShader(squareVS, squareFS));
		}
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

		graphics::RenderCommand::SetClearColour({ 0.85f, 0.35f, 0.f, 0.25f }); // Orange background colour
		graphics::RenderCommand::Clear();

		layerStack.OnUpdate();

		pImGuiLayer->BeginDraw();
		layerStack.OnRender();
		pImGuiLayer->EndDraw();

		{
			const auto& renderer = graphics::Renderer::Reference();
			renderer.BeginScene();

			pSquareShader->Bind();
			pSquareVA->Bind();
			renderer.Submit(*pSquareVA);

			pTriangleShader->Bind();
			pTriangeVA->Bind();
			renderer.Submit(*pTriangeVA);

			renderer.EndScene();
		}

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
