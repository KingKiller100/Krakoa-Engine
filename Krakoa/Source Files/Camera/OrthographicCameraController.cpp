#include "Precompile.hpp"
#include "OrthographicCameraController.hpp"

#include "../Instrumentor.hpp"
#include "../Input/KeyCode.hpp"
#include "../Input/InputManager.hpp"


namespace krakoa
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool isRotationAllowed)
		: aspectRatio(aspectRatio), zoomLevel(1.f), isRotationAllowed(isRotationAllowed),
		camera(-(aspectRatio * zoomLevel), aspectRatio * zoomLevel, -zoomLevel, zoomLevel),
		rotationZ(0.f), camRotationSpeed(1.f), camTranslationSpeed(1.f)
	{
		KRK_PROFILE_FUNCTION();
	}

	OrthographicCameraController::~OrthographicCameraController()
		= default;

	void OrthographicCameraController::OnUpdate(const float deltaTime) noexcept
	{
		KRK_PROFILE_FUNCTION();

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
		UpdateCameraProjectionMat();
		return false;
	}

	bool OrthographicCameraController::OnWindowResizeEvent(const events::WindowResizeEvent& e)
	{
		KRK_PROFILE_FUNCTION();
		if (e.GetHeight() == 0)
			aspectRatio = 0;
		else
		{
			const auto width =  CAST(float, e.GetWidth() );
			const auto height = CAST(float, e.GetHeight());
			aspectRatio = width / height;
		}
		
		UpdateCameraProjectionMat();
		return false;
	}

	void OrthographicCameraController::UpdateCameraProjectionMat() noexcept
	{
		KRK_PROFILE_FUNCTION();
		const auto xVal = aspectRatio * zoomLevel;
		camera.SetProjection(-xVal, xVal, -zoomLevel, zoomLevel);
	}
}