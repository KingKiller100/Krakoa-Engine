#include "Precompile.hpp"
#include "Application.hpp"

#include "Logging/CoreLogger.hpp"

#include "../Input/InputManager.hpp"

#include "../Rendering/LayerBase.hpp"
#include "../Rendering/Renderer.hpp"
#include "../Rendering/Graphics/BufferLayout.hpp"
#include "../Platform/OpenGL/OpenGLShader.hpp"

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

		// Rendering code - should go in renderer;

		// Rendering hardware info
		KRK_INFO("OpenGL info:");
		KRK_INFO(klib::kFormat::ToString("\t Vendor: %s", glGetString(GL_VENDOR)));
		KRK_INFO(klib::kFormat::ToString("\t Renderer: %s", glGetString(GL_RENDERER)));
		KRK_INFO(klib::kFormat::ToString("\t Version: %s", glGetString(GL_VERSION)));

		pTriangeVA = std::unique_ptr<graphics::iVertexArray>(graphics::iVertexArray::Create());

		{
			// Triangle creation code
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


		}



		const std::string_view vertexSource = R"(
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


		const std::string_view fragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 out_color;

			in vec3 v_Position;
			in vec4 v_Colour;

			void main()
			{
				out_color = v_Colour;
			}
		)";

		pShader = std::unique_ptr<graphics::iShader>(new graphics::OpenGLShader(vertexSource, fragmentSource));
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

		pShader->Bind();
		pTriangeVA->Bind();
		glDrawElements(GL_TRIANGLES, pTriangeVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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
