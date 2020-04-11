#include <Krakoa.hpp>

#include <imgui/imgui.h>

class RendererLayer : public krakoa::LayerBase
{
public:
	RendererLayer()
		: LayerBase("Renderer"),
		camera(-1.6f, 1.6f, -1.f, 1.f), // Aspect ratio from window size
		cameraRotation(0.f),
		cameraMoveSpeed(20.f),
		cameraRotateSpeed(100.f)
	{}

	// Inherited via LayerBase
	void OnAttach() override
	{
		krakoa::graphics::Renderer::Create();
		krakoa::graphics::Renderer::Reference().BeginScene(camera);

		pTriangleVA = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());
		pSquareVA = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());

		// Triangle creation code
		{
			// Vertices points
			kmaths::Matrix<float, 3, 7> vertices = {
				{ -0.5f, -0.5f, 0.f, 0.95f, 0.3f, 0.10f, 1.0f },
				{ 0.5f, -0.5f, 0.f, 0.25f, 0.9f, 0.85f, 1.0f },
				{ 0.0f, 0.5f, 0.0f, 0.95f, 0.3f, 0.78f, 1.0f }
			};

			// Vertex buffer
			auto pVertexBuffer = krakoa::graphics::iVertexBuffer::Create(vertices.GetPointerToData(), sizeof(vertices));

			pVertexBuffer->SetLayout({
				{ krakoa::graphics::ShaderDataType::FLOAT3, "a_Position" },
				{ krakoa::graphics::ShaderDataType::FLOAT4, "a_Colour" }
				}
			);

			pTriangleVA->AddVertexBuffer(pVertexBuffer);

			// Index buffer
			uint32_t indices[3] = { 0, 1, 2 };
			pTriangleVA->SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);


			const std::string_view triangleVS = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Colour;

			out vec3 v_Position;
			out vec4 v_Colour;

			uniform mat4 u_vpMat;

			void main()
			{
				v_Colour = a_Colour;
				v_Position = a_Position;
				gl_Position = u_vpMat * vec4(a_Position, 1.0);
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

			pTriangleShader = std::unique_ptr<krakoa::graphics::iShader>(
				krakoa::graphics::iShader::Create(triangleVS, triangleFS)
				);
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
			auto squareVB = krakoa::graphics::iVertexBuffer::Create(squareVertices.GetPointerToData(), sizeof(squareVertices));

			squareVB->SetLayout({
				{ krakoa::graphics::ShaderDataType::FLOAT3, "a_Position" }
				});

			pSquareVA->AddVertexBuffer(squareVB);

			// Index buffer
			uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
			pSquareVA->SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);


			const std::string_view squareVS = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;
			uniform mat4 u_vpMat;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
				gl_Position = u_vpMat * vec4(a_Position, 1.0);
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

			pSquareShader = std::unique_ptr<krakoa::graphics::iShader>(
				krakoa::graphics::iShader::Create(squareVS, squareFS)
				);
		}
	}

	void OnDetach() override
	{
		pTriangleShader->Unbind();
		pTriangleVA->Unbind();
		pSquareShader->Unbind();
		pSquareVA->Unbind();
	}

	void OnUpdate(float deltaTime) override
	{
		DBUG(klib::kFormat::ToString("Delta Time: %.4f", deltaTime));

		MoveCamera(deltaTime);

		krakoa::graphics::RenderCommand::SetClearColour({ 0.85f, 0.35f, 0.f, 0.25f }); // Orange background colour
		krakoa::graphics::RenderCommand::Clear();

		auto& renderer = krakoa::graphics::Renderer::Reference();
		renderer.Submit(*pSquareShader, *pSquareVA);
		renderer.Submit(*pTriangleShader, *pTriangleVA);
		renderer.EndScene();
	}

	void OnRender() override
	{
		ImGui::Begin("Demo");
		ImGui::Text("This is the demo layer's window");
		ImGui::End();
	}

	void OnEvent(krakoa::events::Event& e) override
	{
		//DBUG(e.ToString());
	}

private:
	void MoveCamera(float deltaTime) noexcept
	{
		if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_UP))
			cameraPos.Y() += cameraMoveSpeed * deltaTime;
		else if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_DOWN))
			cameraPos.Y() -= cameraMoveSpeed * deltaTime;

		if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_LEFT))
			cameraPos.X() -= cameraMoveSpeed * deltaTime;
		else if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_RIGHT))
			cameraPos.X() += cameraMoveSpeed * deltaTime;

		if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_A))
			cameraRotation += cameraRotateSpeed * deltaTime;
		else if (krakoa::input::InputManager::IsKeyPressed(KRK_KEY_D))
			cameraRotation -= cameraRotateSpeed * deltaTime;

		camera.SetPosition(cameraPos);
		camera.SetRotation(cameraRotation);
	}


private:
	// To go in Renderer
	std::unique_ptr<krakoa::graphics::iShader> pTriangleShader;
	std::unique_ptr<krakoa::graphics::iShader> pSquareShader;
	std::unique_ptr<krakoa::graphics::iVertexArray> pTriangleVA;
	std::unique_ptr<krakoa::graphics::iVertexArray> pSquareVA;

	krakoa::OrthographicCamera camera;
	kmaths::Vector3f cameraPos;
	float cameraRotation;
	const float cameraMoveSpeed;
	const float cameraRotateSpeed;
};

class Hooper2Game : public krakoa::Application
{
public:
	Hooper2Game(Token&& t)
		: Application(t)
	{
		INIT_LOGS("Hooper2");
		TOGGLE_SUBSYSTEM_LOGGING();
		SET_LOG_MIN(LOG_LVL_DBUG);

		PushLayer(new RendererLayer());
		//PushOverlay(new krakoa::ImGuiLayer());
	}

	~Hooper2Game()
	{
		Hooper2Game::Shutdown();
	}

	void Shutdown() override
	{
		isRunning = false;
	}
};

void krakoa::CreateApplication()
{
	Application::Create<Hooper2Game>();
}