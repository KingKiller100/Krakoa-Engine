#include "Precompile.hpp"
#include "OrthographicCameraController.hpp"

#include "../Input/InputManager.hpp"
#include "../Input/KeyCode.hpp"

namespace krakoa
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool isRotationAllowed)
		: aspectRatio(aspectRatio), zoomLevel(1.f), isRotationAllowed(isRotationAllowed),
		camera(-(aspectRatio * zoomLevel), aspectRatio * zoomLevel, -zoomLevel, zoomLevel),
		rotationZ(0.f), camRotationSpeed(1.f), camTranslationSpeed(1.f)
	{}

	OrthographicCameraController::~OrthographicCameraController()
		= default;

	void OrthographicCameraController::OnUpdate(const float deltaTime) noexcept
	{
		if (input::InputManager::IsKeyPressed(KRK_KEY_W))
			position.Y() += camTranslationSpeed * zoomLevel * deltaTime;
		else if (input::InputManager::IsKeyPressed(KRK_KEY_S))
			position.Y() -= camTranslationSpeed * zoomLevel * deltaTime;

		if (input::InputManager::IsKeyPressed(KRK_KEY_A))
			position.X() -= camTranslationSpeed * zoomLevel* deltaTime;
		else if (input::InputManager::IsKeyPressed(KRK_KEY_D))
			position.X() += camTranslationSpeed * zoomLevel * deltaTime;

		camera.SetPosition(position);

		if (!isRotationAllowed)
			return;

		if (input::InputManager::IsKeyPressed(KRK_KEY_Q))
			rotationZ += camRotationSpeed * deltaTime;
		else if (input::InputManager::IsKeyPressed(KRK_KEY_E))
			rotationZ -= camRotationSpeed * deltaTime;

		camera.SetRotation(rotationZ);
	}

	krakoa::OrthographicCamera& OrthographicCameraController::GetCamera() noexcept
	{
		return camera;
	}

	const krakoa::OrthographicCamera& OrthographicCameraController::GetCamera() const noexcept
	{
		return camera;
	}

	void OrthographicCameraController::SetRotationSpeed(const float rotSpeed) noexcept
	{
		camRotationSpeed = rotSpeed;
	}

	void OrthographicCameraController::SetTranslationSpeed(const float transSpeed) noexcept
	{
		camTranslationSpeed = transSpeed;
	}

	void OrthographicCameraController::OnEvent(events::Event& e)
	{
		events::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<events::MouseScrolledEvent>(KRK_BIND1(OrthographicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<events::WindowResizeEvent>(KRK_BIND1(OrthographicCameraController::OnWindowResizeEvent));
	}

	bool OrthographicCameraController::OnMouseScrolledEvent(const events::MouseScrolledEvent& e)
	{
		zoomLevel -= e.GetY() * 0.05f;
		zoomLevel = kmaths::Clamp(zoomLevel, 0.25f, 5.f);
		UpdateCameraProjectionMat();
		return false;
	}

	bool OrthographicCameraController::OnWindowResizeEvent(const events::WindowResizeEvent& e)
	{
		if (e.GetHeight() != 0.f)
		{
			const float width = e.GetWidth();
			const float height = e.GetHeight();
			aspectRatio = width / height;
		}
		else
			aspectRatio = 0;
		UpdateCameraProjectionMat();
		return false;
	}

	void OrthographicCameraController::UpdateCameraProjectionMat() noexcept
	{
		const auto xVal = aspectRatio * zoomLevel;
		camera.SetProjection(-xVal, xVal, -zoomLevel, zoomLevel);
	}
}