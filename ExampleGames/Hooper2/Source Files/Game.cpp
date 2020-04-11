#include <Krakoa.hpp>

#include <imgui/imgui.h>

#include <Maths/Matrices/MatrixMathsHelper.hpp>

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

		pSquareVA = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());

		// Triangle creation code
		{
			// Vertices points
			kmaths::Matrix3x3f vertices = {
				{ -0.5f, -0.5f, 0.f },
				{ 0.5f, -0.5f, 0.f },
				{ 0.0f, 0.5f, 0.0f }
			};

			// Vertex buffer
			auto pVertexBuffer = krakoa::graphics::iVertexBuffer::Create(vertices.GetPointerToData(), sizeof(vertices));

			pVertexBuffer->SetLayout({
				{ krakoa::graphics::ShaderDataType::FLOAT3, "a_Position" }
				}
			);

			// Index buffer
			uint32_t indices[3] = { 0, 1, 2 };
			pSquareVA->SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);

			const std::string_view vertexShader = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;
			out vec4 v_Colour;
 
			uniform mat4 u_VpMat;
			uniform mat4 u_TransformMat;
			uniform vec4 u_Colour;

			void main()
			{
				v_Colour = u_Colour;
				v_Position = a_Position;
				gl_Position = u_VpMat * u_TransformMat * vec4(a_Position, 1.0);
			}
		)";


			const std::string_view fragmentShader = R"(
			#version 330 core

			layout(location = 0) out vec4 out_color;

			in vec3 v_Position;
			in vec4 v_Colour;

			void main()
			{
				out_color = v_Colour;
			}
		)";

			pColoursShader = std::unique_ptr<krakoa::graphics::iShader>(
				krakoa::graphics::iShader::Create(vertexShader, fragmentShader)
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
		}
	}

	void OnDetach() override
	{
		pColoursShader->Unbind();
		pSquareVA->Unbind();
	}

	void OnUpdate(float deltaTime) override
	{
		DBUG(klib::kFormat::ToString("FPS: %.4f", 1.f / deltaTime));

		MoveCamera(deltaTime);

		krakoa::graphics::RenderCommand::SetClearColour({ 0.85f, 0.35f, 0.f, 0.25f }); // Orange background colour
		krakoa::graphics::RenderCommand::Clear();

		auto& renderer = krakoa::graphics::Renderer::Reference();

		pColoursShader->Bind();
		pColoursShader->UploadUniformVec4("u_Colour", squareColour);

		const auto scale = kmaths::Scale<float>(kmaths::Vector3f(0.1f));

		for (auto y = 0; y < 20; ++y) {
			for (auto x = 0; x < 20; ++x)
			{
				const auto miniSquarePos = kmaths::Vector3f{ x * 2.f, y * 2.f, 0.f };
				const auto miniSquareTransform = kmaths::Translate(miniSquarePos) * scale;
				renderer.Submit(*pColoursShader, *pSquareVA, miniSquareTransform);
			}
		}

		renderer.EndScene();
	}

	void OnRender() override
	{
		ImGui::Begin("Square Settings");
		ImGui::ColorEdit4("Square Colour", squareColour.GetPointerToData());
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
	std::unique_ptr<krakoa::graphics::iShader> pColoursShader;
	std::unique_ptr<krakoa::graphics::iVertexArray> pSquareVA;

	krakoa::OrthographicCamera camera;
	kmaths::Vector3f cameraPos;
	float cameraRotation;

	const float cameraMoveSpeed;
	const float cameraRotateSpeed;

	kmaths::Vector4f squareColour;
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