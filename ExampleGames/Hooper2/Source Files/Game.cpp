#include <Krakoa.hpp>

#include <imgui/imgui.h>

#include <Maths/Matrices/MatrixMathsHelper.hpp>
#include <Maths/kAlgorithms.hpp>

#include <cmath>

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
		krakoa::graphics::Renderer::Reference().BeginScene(camera);

		auto& shaderLib = krakoa::graphics::ShaderLibrary::Reference();

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
			auto triangleVB = krakoa::graphics::iVertexBuffer::Create(vertices.GetPointerToData(), sizeof(vertices));

			triangleVB->SetLayout({
				{ krakoa::graphics::ShaderDataType::FLOAT3, "a_Position" },
				});

			pTriangleVA->AddVertexBuffer(triangleVB);

			// Index buffer
			uint32_t indices[] = { 0, 1, 2 };
			pTriangleVA->SetIndexBuffer(krakoa::graphics::iIndexBuffer::Create(
				indices,
				sizeof(indices) / sizeof(uint32_t))
			);

			shaderLib.Load("../../../ExampleGames/Hooper2/Assets/Shaders/OpenGL/ColourShader");
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
			auto squareVB = krakoa::graphics::iVertexBuffer::Create(
				squareVertices.GetPointerToData(),
				sizeof(squareVertices)
			);

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
		}

		auto& textureShader = shaderLib.Load("Texture", "../../../ExampleGames/Hooper2/Assets/Shaders/OpenGL/TextureShader");

		pWinTexture = std::unique_ptr<krakoa::graphics::iTexture>(
			krakoa::graphics::iTexture2D::Create("Assets/Win.png")
			);

		textureShader.Bind();
		textureShader.UploadUniformInt("u_Texture", 0);
	}

	void OnDetach() override
	{
		pSquareVA->Unbind();
		pTriangleVA->Unbind();
	}

	void OnUpdate(float deltaTime) override
	{
		static size_t count = 0;

		const auto& shaderLib = krakoa::graphics::ShaderLibrary::Reference();
		auto& colourShader = shaderLib.Get("Shader 0");
		auto& textureShader = shaderLib.Get("Texture");


		frameTimes[kmaths::modulus(count++, frameTimes.size())] = (1.f / deltaTime);

		MoveCamera(deltaTime);

		auto& renderer = krakoa::graphics::Renderer::Reference();

		renderer.SetClearColour({ 0.85f, 0.35f, 0.f, 0.25f }); // Orange background colour
		renderer.Clear();

		const auto triangleTransform = kmaths::Translate<float>({ 0.f, 0.f, 0.f }) *
			kmaths::Scale<float>({ 2.f, 2.f, 1.f });

		colourShader.Bind();
		colourShader.UploadUniformVec4("u_Colour", triangleColour);
		renderer.Submit(colourShader, *pTriangleVA, triangleTransform);

		const auto scale = kmaths::Scale<float>(kmaths::Vector3f(0.1f));
		for (auto y = 0; y < 5; ++y) {
			for (auto x = 0; x < 5; ++x)
			{
				const auto miniSquarePos = kmaths::Vector3f{ x * 2.f, y * 2.0f, 0.f };
				const auto miniSquareTransform = kmaths::Translate(miniSquarePos) * scale;
				textureShader.Bind();
				pWinTexture->Bind();
				renderer.Submit(textureShader, *pSquareVA, miniSquareTransform);
			}
		}

		renderer.EndScene();
	}

	void OnRender() override
	{
		ImGui::Begin("Triangle Settings");

		ImGui::ColorEdit4("Triangle Colour", triangleColour.GetPointerToData());

		const auto loops = frameTimes.size();
		decltype(frameTimes)::value_type sum = 0;
		for (auto i = 0; i < loops; ++i)
			sum += frameTimes[i];
		const auto fps = sum / loops;
		ImGui::Text("Average FPS: %d", fps);

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
	std::unique_ptr<krakoa::graphics::iVertexArray> pSquareVA;
	std::unique_ptr<krakoa::graphics::iVertexArray> pTriangleVA;

	std::unique_ptr<krakoa::graphics::iTexture> pWinTexture;
	// ---------------------------------------------------------

	krakoa::OrthographicCamera camera;
	kmaths::Vector3f cameraPos;
	float cameraRotation;
	const float cameraMoveSpeed;
	const float cameraRotateSpeed;

	std::array<unsigned, 20> frameTimes;

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
	}

	~Hooper2Game()
	{
		Hooper2Game::Shutdown();
	}

	void Initialize() override
	{
		Application::Initialize();
		PushLayer(new RendererLayer());
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