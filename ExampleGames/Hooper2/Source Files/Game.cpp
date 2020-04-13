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

	
	void OnAttach() override
	{
		krakoa::graphics::Renderer::Create();
		krakoa::graphics::Renderer::Reference().BeginScene(camera);


		// Triangle creation code
		{
			pTriangleVA = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());

			// Vertices points
			kmaths::Matrix3x3f vertices = {
				{ -0.5f, -0.5f, 0.f },
				{ 0.5f, -0.5f, 0.f },
				{ 0.0f, 0.5f, 0.0f }
			};

			// Vertex buffer
			auto pVertexBuffer = krakoa::graphics::iVertexBuffer::Create(vertices.GetPointerToData(), sizeof(vertices));

			pVertexBuffer->SetLayout({
				{ krakoa::graphics::ShaderDataType::FLOAT3, "a_Position" },
				});

			// Index buffer
			uint32_t indices[3] = { 0, 1, 2 };
			pTriangleVA->SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);

			pColoursShader = std::unique_ptr<krakoa::graphics::iShader>(
				krakoa::graphics::iShader::Create(
					"../../../ExampleGames/Hooper2/Shaders/OpenGL/ColourVertexShader.glsl", // vertex shader source
					"../../../ExampleGames/Hooper2/Shaders/OpenGL/ColourFragmentShader.glsl") // fragment shader source
				);
		}

		// Square creation code
		{
			pSquareVA = std::unique_ptr<krakoa::graphics::iVertexArray>(krakoa::graphics::iVertexArray::Create());

			kmaths::Matrix<float, 4, 5> squareVertices = {
					{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
					{ -0.5f,  0.5f, 0.0f, 0.0f, 1.0f },
					{  0.5f,  0.5f, 0.0f, 1.0f, 1.0f },
					{  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
			};

			// Vertex buffer
			auto squareVB = krakoa::graphics::iVertexBuffer::Create(squareVertices.GetPointerToData(), sizeof(squareVertices));

			squareVB->SetLayout({
				{ krakoa::graphics::ShaderDataType::FLOAT3, "a_Position" },
				{ krakoa::graphics::ShaderDataType::FLOAT2, "a_TexCoord" },
				});

			pSquareVA->AddVertexBuffer(squareVB);

			// Index buffer
			uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
			pSquareVA->SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);

			pTextureShader = std::unique_ptr<krakoa::graphics::iShader>(
				krakoa::graphics::iShader::Create(
					"../../../ExampleGames/Hooper2/Shaders/OpenGL/TextureVertexShader.glsl", // vertex shader source
					"../../../ExampleGames/Hooper2/Shaders/OpenGL/TextureFragmentShader.glsl") // fragment shader source
				);
		}
	}

	void OnDetach() override
	{
		pTextureShader->Unbind();
		pSquareVA->Unbind();

		pColoursShader->Unbind();
		pTriangleVA->Unbind();
	}

	void OnUpdate(float deltaTime) override
	{
		fps = unsigned(1.f / deltaTime);

		MoveCamera(deltaTime);

		krakoa::graphics::RenderCommand::SetClearColour({ 0.85f, 0.35f, 0.f, 0.25f }); // Orange background colour
		krakoa::graphics::RenderCommand::Clear();

		auto& renderer = krakoa::graphics::Renderer::Reference();

		const auto scale = kmaths::Scale<float>(kmaths::Vector3f(0.1f));
		for (auto y = 0; y < 5; ++y) {
			for (auto x = 0; x < 5; ++x)
			{
				const auto miniSquarePos = kmaths::Vector3f{ x * 0.2f, y * 0.2f, 0.f };
				const auto miniSquareTransform = kmaths::Translate(miniSquarePos) * scale;
				renderer.Submit(*pTextureShader, *pSquareVA, miniSquareTransform);
			}
		}

		pColoursShader->Bind();
		pColoursShader->UploadUniformVec4("u_Colour", triangleColour);
		renderer.Submit(*pColoursShader, *pTriangleVA);

		renderer.EndScene();
	}

	void OnRender() override
	{
		ImGui::Begin("Triangle Settings");
		ImGui::ColorEdit4("Triangle Colour", triangleColour.GetPointerToData());
		ImGui::Text("FPS: %d", fps);
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
	// Temp ----------------------------------------------------
	std::unique_ptr<krakoa::graphics::iShader> pColoursShader;
	std::unique_ptr<krakoa::graphics::iShader> pTextureShader;

	std::unique_ptr<krakoa::graphics::iVertexArray> pSquareVA;
	std::unique_ptr<krakoa::graphics::iVertexArray> pTriangleVA;
	// ---------------------------------------------------------

	krakoa::OrthographicCamera camera;
	kmaths::Vector3f cameraPos;
	float cameraRotation;
	const float cameraMoveSpeed;
	const float cameraRotateSpeed;

	unsigned fps;

	kmaths::Vector4f triangleColour;
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