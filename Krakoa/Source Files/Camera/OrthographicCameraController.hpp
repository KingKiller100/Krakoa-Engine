#pragma once

#include "OrthographicCamera.hpp"
#include "../Events System/MouseEvent.hpp"
#include "../Events System/ApplicationEvent.hpp"

#include <Maths/Vectors/Vector3.hpp>

namespace krakoa
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool isRotationAllowed = false);
		~OrthographicCameraController();

		void OnUpdate(const float deltaTime) noexcept;
		void OnEvent(events::Event& e);

		OrthographicCamera& GetCamera() noexcept;
		const OrthographicCamera& GetCamera() const noexcept;

		void SetRotationSpeed(const float rotSpeed) noexcept;
		void SetTranslationSpeed(const float transSpeed) noexcept;

	private:
		bool OnMouseScrolledEvent(const events::MouseScrolledEvent& e);
		bool OnWindowResizeEvent(const events::WindowResizeEvent& e);

		void UpdateCameraProjectionMat() noexcept;

	private:
		float aspectRatio; // wdith / height
		float zoomLevel;
		bool isRotationAllowed;
		OrthographicCamera camera;

		float rotationZ;
		kmaths::Vector3f position;

		float camRotationSpeed;
		float camTranslationSpeed;
	};
}