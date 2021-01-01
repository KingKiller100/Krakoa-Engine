#pragma once

#include "OrthographicCamera.hpp"
#include "../EventsSystem/MouseEvent.hpp"
#include "../EventsSystem/ApplicationEvent.hpp"

#include <Maths/Vectors/Vector3.hpp>

namespace krakoa
{
	class OrthographicCameraController
	{
		struct Bounds
		{
			float left = 0, right = 1, bottom = 0, top = 1;

			USE_RESULT constexpr float GetWidth() const  { return (right - left); }
			USE_RESULT constexpr float GetHeight() const { return (top - bottom); }
		};
	public:
		OrthographicCameraController(float aspectRatio, bool isRotationAllowed = false);
		~OrthographicCameraController();

		void OnUpdate(const float deltaTime) noexcept;
		void OnEvent(events::Event& e);

		OrthographicCamera& GetCamera() noexcept;
		const OrthographicCamera& GetCamera() const noexcept;

		void SetRotationSpeed(const float rotSpeed) noexcept;
		void SetTranslationSpeed(const float transSpeed) noexcept;

		const Bounds& GetBounds() const;

	private:
		bool OnMouseScrolledEvent(const events::MouseScrolledEvent& e);
		bool OnWindowResizeEvent(const events::WindowResizeEvent& e);

		void UpdateCameraProjectionMat() noexcept;

	private:
		float aspectRatio; // width / height
		float zoomLevel;
		bool isRotationAllowed;
		Bounds bounds;
		OrthographicCamera camera;

		float rotationZ;
		kmaths::Vector3f position;

		float camRotationSpeed;
		float camTranslationSpeed;
	};
}