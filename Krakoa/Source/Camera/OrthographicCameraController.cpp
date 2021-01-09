#include "Precompile.hpp"
#include "OrthographicCameraController.hpp"

#include "../Debug/Instrumentor.hpp"
#include "../Input/KeyCode.hpp"
#include "../Input/InputManager.hpp"


namespace krakoa
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool isRotationAllowed)
		: aspectRatio(aspectRatio), zoomLevel(1.f), isRotationAllowed(isRotationAllowed),
		bounds({ -(aspectRatio * zoomLevel), aspectRatio * zoomLevel, -zoomLevel, zoomLevel }),
		camera(bounds.left, bounds.right, bounds.bottom, bounds.top),
		rotationZ(0.f), camRotationSpeed(1.f), camTranslationSpeed(1.f)
	{
		KRK_PROFILE_FUNCTION();
	}

	OrthographicCameraController::~OrthographicCameraController()
		= default;

	void OrthographicCameraController::OnUpdate(const float deltaTime) noexcept
	{
		KRK_PROFILE_FUNCTION();

		if (input::InputManager::IsKeyPressed(input::KEY_W))
			position.Y() += camTranslationSpeed * zoomLevel * deltaTime;
		else if (input::InputManager::IsKeyPressed(input::KEY_S))
			position.Y() -= camTranslationSpeed * zoomLevel * deltaTime;

		if (input::InputManager::IsKeyPressed(input::KEY_A))
			position.X() -= camTranslationSpeed * zoomLevel * deltaTime;
		else if (input::InputManager::IsKeyPressed(input::KEY_D))
			position.X() += camTranslationSpeed * zoomLevel * deltaTime;

		camera.SetPosition(position);

		if (!isRotationAllowed)
			return;

		if (input::InputManager::IsKeyPressed(input::KEY_Q))
			rotationZ += camRotationSpeed * deltaTime;
		else if (input::InputManager::IsKeyPressed(input::KEY_E))
			rotationZ -= camRotationSpeed * deltaTime;

		camera.SetRotation(rotationZ);
	}

	krakoa::OrthographicCamera& OrthographicCameraController::GetCamera() noexcept
	{
		KRK_PROFILE_FUNCTION();
		return camera;
	}

	const krakoa::OrthographicCamera& OrthographicCameraController::GetCamera() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return camera;
	}

	void OrthographicCameraController::SetRotationSpeed(const float rotSpeed) noexcept
	{
		KRK_PROFILE_FUNCTION();
		camRotationSpeed = rotSpeed;
	}

	void OrthographicCameraController::SetTranslationSpeed(const float transSpeed) noexcept
	{
		KRK_PROFILE_FUNCTION();
		camTranslationSpeed = transSpeed;
	}

	const OrthographicCameraController::Bounds& OrthographicCameraController::GetBounds() const
	{
		return bounds;
	}

	void OrthographicCameraController::OnEvent(events::Event& e)
	{
		KRK_PROFILE_FUNCTION();
		events::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<events::MouseScrolledEvent>(KRK_BIND1(OrthographicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<events::WindowResizeEvent>(KRK_BIND1(OrthographicCameraController::OnWindowResizeEvent));
	}

	bool OrthographicCameraController::OnMouseScrolledEvent(const events::MouseScrolledEvent& e)
	{
		KRK_PROFILE_FUNCTION();
		zoomLevel -= e.GetY() * 0.05f;
		zoomLevel = kmaths::Clamp(zoomLevel, 0.25f, 5.f);

		const auto xVal = aspectRatio * zoomLevel;
		bounds = { -xVal, xVal, -zoomLevel, zoomLevel };
		UpdateCameraProjectionMat();
		return false;
	}

	bool OrthographicCameraController::OnWindowResizeEvent(const events::WindowResizeEvent& e)
	{
		KRK_PROFILE_FUNCTION();
		Resize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
		return false;
	}

	void OrthographicCameraController::Resize(kmaths::Vector2f dimensions)
	{
		Resize(dimensions.x, dimensions.y);
	}

	void OrthographicCameraController::Resize(float width, float height)
	{
		if (height == 0)
			aspectRatio = 0;
		else
		{
			aspectRatio = width / height;
		}

		const auto xVal = aspectRatio * zoomLevel;

		bounds = { -xVal, xVal, -zoomLevel, zoomLevel };

		UpdateCameraProjectionMat();
	}


	void OrthographicCameraController::UpdateCameraProjectionMat() noexcept
	{
		KRK_PROFILE_FUNCTION();
		camera.SetProjection(bounds.left, bounds.right, bounds.bottom, bounds.top);
	}
}